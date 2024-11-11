#ifndef CE4302_SIMPLE_MEM_H
#define CE4302_SIMPLE_MEM_H

#include "Interfaces.hpp"

constexpr int simple_memory_size{4096};

class SimpleMemory: public MemorySlave
{
private:

    int64_t memory[simple_memory_size];
public:
    int64_t read_request(int64_t addr) override; 
    void write_request(int64_t addr, int64_t values) override; 
};

#endif 