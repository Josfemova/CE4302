#include "BusInterconnect.hpp"

#include "messages.hpp"
#include <algorithm>

void BusInterconnect::register_mem_slave(MemorySlave *mem_slave,
                                         int64_t start_addr, int64_t end_addr)
{
    MemorySlaveCard card = {
        .ptr = mem_slave, .start_addr = start_addr, .end_addr = end_addr};
    this->mem_slaves.push_back(card);
}

void BusInterconnect::register_bus_master(BusMaster *mem_master)
{
    BusMasterCard card = {
        .ptr = mem_master, .read_requests = 0, .write_requests = 0};
    this->bus_masters.push_back(card);
}

void BusInterconnect::update()
{
    std::lock_guard<std::mutex> bus_guard{bus_mutex};
    if (!this->bus_active)
    {
        // si el bus no está activo, se puede cambiar el master
        switch (this->arb_policy)
        {
        case ArbitrationPolicy::RoundRobin:
        {
            if (this->current_master_index !=
                ((int)this->bus_masters.size() - 1))
            {
                this->current_master_index += 1;
            }
            else
            {
                this->current_master_index = 0;
            }

            BusMaster *current_master =
                bus_masters[this->current_master_index].ptr;
            // buscar si el master actual está asociado a un request
            auto master_id_it =
                std::find(this->request_queue.begin(),
                          this->request_queue.end(), current_master->get_id());

            if (master_id_it != this->request_queue.end())
            {
                // hay un request activo del master actual seleccionado
                this->request_queue.erase({master_id_it});
                this->current_master_id = current_master->get_id();
                this->bus_active = true;
            }

            break;
        }
        case ArbitrationPolicy::FIFO:
        default:
            if (!this->request_queue.empty())
            {
                this->current_master_id = this->request_queue.front();
                this->request_queue.pop_front();
                this->bus_active = true;
            }
            break;
        }
    }
}

int64_t BusInterconnect::read_main_memory(int64_t addr)
{
    auto selected_slave_it = find_if(
        this->mem_slaves.begin(), this->mem_slaves.end(), [=](auto msc) -> bool
        { return (addr >= msc.start_addr) && (addr <= msc.end_addr); });
    if (selected_slave_it == this->mem_slaves.end())
    {
        throw 30; // si llega acá es que la dirección ta fuera de memoria
    }
    else
    {
        int local_addr = addr - (*selected_slave_it).start_addr;
        return (*selected_slave_it).ptr->read_request(local_addr);
    }
}

void BusInterconnect::write_main_memory(int64_t addr, int64_t value)
{
    this->write_reqs++;
    auto selected_slave_it = find_if(
        this->mem_slaves.begin(), this->mem_slaves.end(), [=](auto msc) -> bool
        { return (addr >= msc.start_addr) && (addr <= msc.end_addr); });
    if (selected_slave_it == this->mem_slaves.end())
    {
        throw 30; // si llega acá es que la dirección ta fuera de memoria
    }
    else
    {
        int local_addr = addr - (*selected_slave_it).start_addr;
        (*selected_slave_it).ptr->write_request(local_addr, value);
    }
}

bool BusInterconnect::bus_request(BusMessage_t &request)
{
    bus_mutex.lock();
    this->request_queue.push_back(request.master_id);
    bus_mutex.unlock();

    // esperar al turno del cliente
    bool not_ready = true;
    while (not_ready)
    {
        {
            std::lock_guard<std::mutex> bus_guard{bus_mutex};
            not_ready = (this->current_master_id != request.master_id) ||
                        (!this->bus_active);
        }
        this->step();
        if (this->abort)
        {
            return false;
        }
    }

    this->step(); // flecha entrante de caché

    //======================
    // Service request
    //======================

    notify::interconnect_event(request);

    // actualizar contadores
    if ((request.type == BusMessageType::BusRdX) ||
        (request.type == BusMessageType::BusUpgr))
    {
        invalidations++;
    }
    bool is_rd = false;
    if ((request.type == BusMessageType::BusRd) ||
        (request.type == BusMessageType::BusRdX))
    {
        read_reqs++;
        is_rd = true;
        this->pe_data_tx[request.master_id] += 32; //(32 bytes)
    }

    if (request.type == BusMessageType::Flush)
    {
        write_reqs++;
    }

    // Notify each bus master of the request
    for (auto bmc : this->bus_masters)
    {
        bool was_complete = request.completed;
        bmc.ptr->handle_bus_message(request);
        if (was_complete != request.completed)
        {
            read_resp += is_rd ? 1 : 0;
        }
    }

    if (request.completed == false)
    {
        switch (request.type)
        {
        case BusMessageType::BusRd:
        case BusMessageType::BusRdX:
            for (int i = 0; i < 4; i++)
            {
                request.data[i] =
                    this->read_main_memory(request.address + i * 8);
                this->main_mem_reads++;
                this->read_resp++;
            }
            request.exclusive = true;
            break;
        case BusMessageType::Flush:
            for (int i = 0; i < 4; i++)
            {
                this->write_main_memory(request.address + i * 8,
                                        request.data[i]);
                this->main_mem_writes++;
                this->write_resp++;
            }
            break;
        case BusMessageType::BusUpgr:
        default:
            break;
        }
        request.completed = true;
    }
    notify::bus_interconnect_update(*this);
    // no necesita hacer lock del mutex porque no hay condicion de carrera
    // posible
    this->step(); // flecha saliente a caché
    this->bus_active = false;
    return true;
}

BusInterconnect::BusInterconnect()
    : Clocked(), arb_policy{ArbitrationPolicy::FIFO}, current_master_index{0},
      current_master_id{0}, bus_active{false}, invalidations{}, read_reqs{},
      read_resp{}, write_reqs{}, write_resp{}, abort{false}, main_mem_reads{},
      main_mem_writes{}, pe_data_tx{}
{
}

void BusInterconnect::abort_exec() { this->abort = true; }
