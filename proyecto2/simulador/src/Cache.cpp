#include "Cache.hpp"
#include "messages.hpp"
#include <algorithm>
#include <format>
#include <iostream>

int Cache::current_id = 0;

Cache::Cache(Bus &bus, int id)
    : Clocked(), id{id}, cache{}, invalidations{}, cache_misses{}, bus{bus},
      flushed{false}
{
    bus.register_bus_master(this);
    int index = 0;
    for (const auto &l : this->cache)
    {
        notify::update_cache(id, l, index);
        index++;
    }
}

Cache::Cache(Bus &bus)
    : Clocked(), id{this->current_id++}, cache{}, invalidations{},
      cache_misses{}, bus{bus}, flushed{false}
{
    bus.register_bus_master(this);
    int index = 0;
    for (const auto &l : this->cache)
    {
        notify::update_cache(id, l, index);
        index++;
    }
}

/// @brief retorna el indice de la linea de cache
CacheLine &Cache::cacheline_check(int64_t addr)
{
    int64_t tag = cacheline_tag(addr);
    int64_t index = cacheline_index(addr);
    auto &sel_cacheline = this->cache[index];

    if (sel_cacheline.tag != tag)
    {
        // cache miss
        this->cache_misses += 1;
        if (sel_cacheline.state == MESIState::Invalid)
        {
            return sel_cacheline;
        }
        int64_t flush_addr = cacheline_addr(sel_cacheline.tag, index);
        sel_cacheline.state = MESIState::Invalid;
        this->step(); // penalización de un ciclo por el miss
        notify::update_cache(this->id, sel_cacheline, index);
        BusMessage_t request = {.type = BusMessageType::Flush,
                                .address = flush_addr,
                                .data = sel_cacheline.data,
                                .master_id = this->id,
                                .exclusive = true,
                                .completed = false};
        // escribir datos de linea a reemplazar a memoria principal
        this->bus.bus_request(request);
        notify::update_cache(this->id, sel_cacheline, index);
    }
    return sel_cacheline;
}

int64_t Cache::read_request(int64_t addr)
{
    this->step(); // flecha entrante del PE
    this->bus.request_bus_access(
        this); // no procesar request hasta que bus chequee eventos
    auto &cline = this->cacheline_check(addr);
    int64_t offset = cacheline_offset(addr); // que multiplo de 8 es

    if (cline.state == MESIState::Invalid)
    {
        BusMessage_t request = {.type = BusMessageType::BusRd,
                                .address = aligned32_addr(addr),
                                .data = cline.data,
                                .master_id = this->id,
                                .exclusive = true,
                                .completed = false};

        this->bus.bus_request(request);
        cline.state =
            (request.exclusive) ? MESIState::Exclusive : MESIState::Shared;
        cline.tag = cacheline_tag(addr);
        int64_t index = cacheline_index(addr);
        notify::update_cache(this->id, cline, index);
    }
    this->step(); // flecha saliente al PE
    this->bus.yield_bus_access(this);
    return cline.data[offset];
}

void Cache::write_request(int64_t addr, int64_t value)
{
    this->step(); // flecha entrante del PE
    this->bus.request_bus_access(
        this); // no procesar request hasta que bus chequee eventos
    auto &cline = this->cacheline_check(addr);
    int64_t offset = cacheline_offset(addr); // que multiplo de 8 es

    BusMessage_t request = {.type = BusMessageType::BusRdX,
                            .address = aligned32_addr(addr),
                            .data = cline.data,
                            .master_id = this->id,
                            .exclusive = true,
                            .completed = false};

    switch (cline.state)
    {
    case MESIState::Invalid:
        this->bus.bus_request(request);
        cline.state = MESIState::Modified;
        break;
    case MESIState::Exclusive:
        cline.state = MESIState::Modified;
        break;
    case MESIState::Shared:
        request.type = BusMessageType::BusUpgr;
        this->bus.bus_request(request);
        cline.state = MESIState::Modified;
        break;
    case MESIState::Modified:
    default:
        break;
    }
    this->step(); // ciclo para que se vea resultado
    cline.data[offset] = value;
    cline.tag = cacheline_tag(addr);
    int64_t index = cacheline_index(addr);
    this->bus.yield_bus_access(this);
    notify::update_cache(this->id, cline, index);
}

int Cache::get_id() { return this->id; }

void Cache::handle_bus_message(BusMessage_t &msg)
{
    this->step(); // flecha del bus al cache
    if (msg.master_id == this->id)
    {
        return; // no hay que administrar requests propios
    }

    // encontrar el cacheline
    int64_t tag = cacheline_tag(msg.address);
    int64_t index = cacheline_index(msg.address);
    auto &cline = this->cache[index];
    // std::cout << std::format("compare tags {} {}\n",tag,tag);
    if (cline.tag != tag)
    {
        return; // no se tiene una copia del cache
    }

    if (this->id == 3 && msg.address == 8)
    {
        std::cout << "c";
    }

    auto old_cline_state = cline.state;
    bool flush_opt = false;
    switch (cline.state)
    {
    case MESIState::Exclusive:
        switch (msg.type)
        {
        case BusMessageType::BusRd:
            // Copiar datos de cache como respuesta (FlushOpt)
            std::copy(cline.data, cline.data + 4, msg.data);
            flush_opt = true;

            // cambio de estado
            cline.state = MESIState::Shared;
            msg.exclusive = false;
            msg.completed = true;
            break;
        case BusMessageType::BusRdX:
            // Copiar datos de cache como respuesta (FlushOpt)
            std::copy(cline.data, cline.data + 4, msg.data);
            flush_opt = true;

            // cambio de estado
            cline.state = MESIState::Invalid;
            msg.exclusive = true;
            msg.completed = true;
            break;
        case BusMessageType::BusUpgr:
        case BusMessageType::Flush:
        default:
            // N/A
            break;
        }
        break;
    case MESIState::Shared:
        switch (msg.type)
        {
        case BusMessageType::BusRd:
            // Copiar datos de cache como respuesta (FlushOpt)
            if (!msg.completed)
            {
                std::copy(cline.data, cline.data + 4, msg.data);
                flush_opt = true;
                msg.exclusive = false;
                msg.completed = true;
            }
            break;
        case BusMessageType::BusRdX:
        case BusMessageType::BusUpgr:
            // cambio de estado
            cline.state = MESIState::Invalid;
            break;
        case BusMessageType::Flush:
        default:
            // N/A
            break;
        }
        break;
    case MESIState::Modified:
        switch (msg.type)
        {
        case BusMessageType::BusRd:
        case BusMessageType::BusRdX:
            // Copiar datos de cache como respuesta (FlushOpt)
            std::copy(cline.data, cline.data + 4, msg.data);
            flush_opt = true;

            // cambio de estado
            if (msg.type == BusMessageType::BusRdX)
            {
                cline.state = MESIState::Invalid;
                msg.exclusive = true;
            }
            else
            {
                cline.state = MESIState::Shared;
                msg.exclusive = false;
            }

            // Causar flush de datos a memoria principal:
            msg.completed = false;
            msg.type = BusMessageType::Flush;
            break;

        case BusMessageType::BusUpgr:
        case BusMessageType::Flush:
        default:
            // N/A
            break;
        }
        break;
    case MESIState::Invalid:
    default:
        break; // Se ignora cualquier mensaje
    }
    if ((old_cline_state != MESIState::Invalid) &&
        (cline.state == MESIState::Invalid))
    {
        this->invalidations++;
    }
    if (old_cline_state != cline.state)
    {
        notify::update_cache(this->id, cline, index);
    }
    if (flush_opt)
    {
        notify::flush_opt(msg, this->id);
    }
    this->step(); // flecha de vuelta al bus
}

void Cache::flush_all()
{
    this->bus.request_bus_access(this);
    int index = 0;
    for (auto &c : this->cache)
    {
        if (c.state == MESIState::Exclusive || c.state == MESIState::Modified)
        {
            BusMessage_t request = {
                .type = BusMessageType::Flush,
                .address = aligned32_addr(cacheline_addr(c.tag, index)),
                .data = c.data,
                .master_id = this->id,
                .exclusive = true,
                .completed = false};
            // escribir datos de linea a reemplazar a memoria principal
            this->bus.bus_request(request);
        }
        index++;
    }
    this->bus.yield_bus_access(this);
    flushed = true;
}