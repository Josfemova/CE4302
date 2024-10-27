#include "SimpleMemory.hpp"

int64_t SimpleMemory::read_request(int64_t addr) {
    int index = addr/8;
    if (index > (simple_memory_size-1)){
        throw 22;
    }else{
        return this->memory[index];
    }
} 

void SimpleMemory::write_request(int64_t addr, int64_t value) {
    int index = addr/8;
    if (index > (simple_memory_size-1)){
        throw 22;
    }else{
        this->memory[index] = value;
    }
}