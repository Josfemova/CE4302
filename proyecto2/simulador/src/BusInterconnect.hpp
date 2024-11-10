#ifndef CE4302_BUS_H
#define CE4302_BUS_H

#include "Interfaces.hpp"
#include <vector>
#include <list>
#include <mutex>
#include <atomic>

enum class ArbitrationPolicy{
    FIFO, 
    RoundRobin
};

class BusInterconnect: public Clocked, public Bus
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


    // funciones privadas
    MemorySlaveCard* resolve_addr(int64_t addr);
    int64_t read_main_memory(int64_t addr); 
    void write_main_memory(int64_t addr, int64_t value);
    std::atomic_bool abort;
public:
    // contadores
    int invalidations;
    int read_reqs;
    int read_resp;
    int write_reqs;
    int write_resp;

    BusInterconnect();
    void register_mem_slave(MemorySlave* mem_slave, int64_t start_addr, int64_t end_addr);
    void register_bus_master(BusMaster* mem_master); 
    /// @brief Corre el algoritmo de arbitraje de bus
    void update();
    bool bus_request(BusMessage_t& request) override; 
    void abort_exec();
};


#endif 