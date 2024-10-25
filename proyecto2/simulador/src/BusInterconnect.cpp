#include "BusInterconnect.hpp"

#include <algorithm>

void BusInterconnect::register_mem_slave(MemorySlave* mem_slave,
                                         word start_addr, word end_addr) {
  MemorySlaveCard card = {
      .ptr = mem_slave, .start_addr = start_addr, .end_addr = end_addr};
  this->mem_slaves.push_back(card);
}

void BusInterconnect::register_mem_master(MemoryMaster* mem_master) {
  MemoryMasterCard card = {
      .ptr = mem_master, .read_requests = 0, .write_requests = 0};
  this->mem_masters.push_back(card);
}

void BusInterconnect::Update() {
  bus_mutex.lock();
  if (!this->bus_active) {
    // si el bus no está activo, se puede cambiar el master
    switch (this->arb_policy) {
      case ArbitrationPolicy::RoundRobin:

        if (this->current_master_index != (this->mem_masters.size() - 1)) {
          this->current_master_index += 1;
        } else {
          this->current_master_index = 0;
        }

        MemoryMaster* current_master =
            mem_masters[this->current_master_index].ptr;
        // buscar si el master actual está asociado a un request
        auto master_id_it =
            std::find(this->request_queue.begin(), this->request_queue.end(),
                      current_master->get_id());

        if (master_id_it != this->request_queue.end()) {
          // hay un request activo del master actual seleccionado
          this->request_queue.erase({master_id_it});
          this->current_master_id = current_master->get_id();
          this->bus_active = true;
        }

        break;
      case ArbitrationPolicy::FIFO:
      default:
        if (!this->request_queue.empty()) {
          this->current_master_id = this->request_queue.front();
          this->request_queue.pop_front();
          this->bus_active = true;
        }
        break;
    }
  }
  bus_mutex.unlock();
}

std::vector<word> BusInterconnect::service_read_request(word addr,
                                                        int burst_size) {
  // enviar mensaje de que se solicita un valor.

  // Log de que se puso el mensaje de read en el bus
}

void BusInterconnect::service_write_request(word addr,
                                            std::vector<word> values) {
  // enviar mensaje de invalidación a todos los masters registrados
  clock->step();
  //
}

std::vector<word> BusInterconnect::read_request(word addr, int burst_size,
                                                int from_ID) {
  bus_mutex.lock();
  this->request_queue.push_back(from_ID);
  bus_mutex.unlock();

  // esperar al turno del cliente
  while (this->current_master_id != from_ID);

  auto ret = service_read_request(addr, burst_size);

  // no necesita hacer lock del mutex porque no hay condicion de carrera posible
  this->bus_active = false;
  return ret;
}

void BusInterconnect::write_request(word addr, std::vector<word> values,
                                    int from_ID) {
  bus_mutex.lock();
  this->request_queue.push_back(from_ID);
  bus_mutex.unlock();

  // esperar al turno del cliente
  while (this->current_master_id != from_ID);

  service_write_request(addr, values);

  // no necesita hacer lock del mutex porque no hay condicion de carrera posible
  this->bus_active = false;
}

BusInterconnect::BusInterconnect(Clock* clock) : clock{clock} {}
