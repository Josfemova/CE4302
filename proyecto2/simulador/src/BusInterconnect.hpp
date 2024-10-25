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

class BusInterconnect: public MemoryMaster, public MemorySlave
{
private:
    std::vector<MemorySlaveCard> mem_slaves;
    std::vector<MemoryMasterCard> mem_masters;


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
    std::vector<word> service_read_request(word addr, int burst_size); 
    void service_write_request(word addr, std::vector<word> values);

public:
    BusInterconnect(Clock* clock);
    void register_mem_slave(MemorySlave* mem_slave, word start_addr, word end_addr) override;
    void register_mem_master(MemoryMaster* mem_master); 
    /**
     * @brief Corre el algoritmo de arbitraje de bus
     * 
     */
    void Update();
    std::vector<word> read_request(word addr, int burst_size, int from_ID) override; 
    void write_request(word addr, std::vector<word> values, int from_ID) override; 
};


#endif 