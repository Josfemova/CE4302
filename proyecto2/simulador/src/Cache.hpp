#ifndef CE4302_CACHE_H
#define CE4302_CACHE_H

#include "Interfaces.hpp"

class Cache: public MemorySlave, public MemoryMaster{
private:
    int cache_misses;
    int invalidations;
public:
};

#endif 