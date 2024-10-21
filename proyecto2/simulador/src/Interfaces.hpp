#ifndef CE4302_INTERFACES_H
#define CE4302_INTERFACES_H

#include "types.hpp"

class MemoryMaster;

class MemorySlave{
public: 
    virtual std::vector<word> read_request(word addr, int burst_size, int from_ID)=0; 
    virtual void write_request(word addr, word value, int from_ID)=0; 
};

typedef struct {
    MemorySlave* ptr;
    word start_addr;
    word end_addr; 
} MemorySlaveCard;

typedef struct {
    MemoryMaster* ptr;
    int read_requests;
    int write_requests; 
} MemoryMasterCard;

enum class BusMessageType{
    ReadRequest,
    ReadExclusiveRequest,
    Invalidate,
    
};

typedef struct{
    int todo;
} BusMessage;

class MemoryMaster{
public:
    virtual int get_id()=0;
    virtual void register_mem_slave(MemorySlave* mem_slave, word start_addr, word end_addr)=0;
    virtual void handle_bus_message(BusMessage& msg)=0;
};

#endif 
