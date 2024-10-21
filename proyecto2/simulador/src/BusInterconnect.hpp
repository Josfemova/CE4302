#ifndef CE4302_BUS_H
#define CE4302_BUS_H

#include "Interfaces.hpp"
#include "Clock.hpp"
#include <vector>
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
    std::mutex transaction_queue_mutex;

    // contadores
    int invalidations;
    Clock* clock;

    word service_read_request(word addr); 
    void service_write_request(word addr, word value);

public:
    BusInterconnect(Clock* clock);
    void register_mem_slave(MemorySlave* mem_slave, word start_addr, word end_addr) override;
    void register_mem_master(MemoryMaster* mem_master); 
    std::vector<word> read_request(word addr, int burst_size, int from_ID) override; 
    void write_request(word addr, word value, int from_ID) override; 
};


#endif 