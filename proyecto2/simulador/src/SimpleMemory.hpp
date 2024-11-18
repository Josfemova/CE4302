#ifndef CE4302_SIMPLE_MEM_H
#define CE4302_SIMPLE_MEM_H

#include <fstream>
#include <sstream>
#include <string>
#include "Interfaces.hpp"

constexpr int simple_memory_size{256};

class SimpleMemory: public MemorySlave, public Clocked
{
private:
    int64_t memory[simple_memory_size];
public:
    SimpleMemory();
    int64_t read_request(int64_t addr) override; 
    void write_request(int64_t addr, int64_t values) override; 
    void load_data(const std::string &filename);
};

#endif 