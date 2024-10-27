#ifndef CE4302_CACHE_H
#define CE4302_CACHE_H

#include "Interfaces.hpp"

enum class MesiState{
    Invalid = 0, 
    Exclusive,
    Shared, 
    Modified
};

typedef struct {
    int64_t data[4]; // 32 bytes
    int64_t tag; // full asociativa, tag es addr[63:6]
    MesiState state;
}CacheLine;



class Cache: public MemorySlave, public BusMaster{
private:
    int id;
    CacheLine cache[8]; 
    int invalidations;
    int cache_misses;
    Bus& bus;

    static int current_id;

public:
    explicit Cache(Bus& bus);
    int64_t read_request(int64_t addr) override; 
    void write_request(int64_t addr, int64_t value) override;
    
    // métodos de bus master 
    virtual int get_id() override;
    virtual void handle_bus_message(BusMessage_t& msg) override;
};

#endif 