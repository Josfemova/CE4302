#ifndef CE4302_BUS_H
#define CE4302_BUS_H

#include "Interfaces.hpp"
#include "Clock.hpp"
#include <vector>
#include <list>
#include <mutex>

enum class ArbitrationPolicy{
    FIFO, 
    RoundRobin
};

class BusInterconnect: public Bus
{
private:
    std::vector<MemorySlaveCard> mem_slaves;
    std::vector<BusMasterCard> bus_masters;


    ArbitrationPolicy arb_policy;
    int current_master_index; // indica el indice de la carta del master actual del bus
    int current_master_id; // indica el id del master actual del bus
    bool bus_active; //  señala si el master actual desocupó el bus
    std::mutex bus_mutex;
    std::list<int> request_queue;


    // contadores
    int invalidations;
    Clock* clock;


    // funciones privadas
    MemorySlaveCard* resolve_addr(int64_t addr);
    int64_t read_main_memory(int64_t addr); 
    void write_main_memory(int64_t addr, int64_t value);

public:
    explicit BusInterconnect(Clock* clock);
    void register_mem_slave(MemorySlave* mem_slave, int64_t start_addr, int64_t end_addr);
    void register_bus_master(BusMaster* mem_master); 
    /// @brief Corre el algoritmo de arbitraje de bus
    void Update();
    void bus_request(BusMessage_t& request) override; 
};


#endif 