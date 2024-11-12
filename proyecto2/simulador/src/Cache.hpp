#ifndef CE4302_CACHE_H
#define CE4302_CACHE_H

#include "Interfaces.hpp"

enum class MESIState{
    Invalid = 0, 
    Exclusive,
    Shared, 
    Modified
};

inline char mesi_state_to_char(MESIState state){
    switch(state){
        case MESIState::Exclusive: return 'E';
        case MESIState::Shared: return 'S';
        case MESIState::Modified: return  'M';
        default: return 'I';
    }
}

typedef struct {
    int64_t data[4]; // 32 bytes
    int64_t tag; // corresp. directa, tag es addr >> 7, offset es addr >> 5 & 0b111
    MESIState state;
}CacheLine;

inline int64_t cacheline_offset(int64_t addr){
    return addr & 0b11; // cual de las 4 palabras de 64bits es 
}

// indica cual de las 4 lineas de cache se selecciona (indice para "cache")
inline int64_t cacheline_index(int64_t addr){
    return (addr >> 2) & 0b111;
}

inline int64_t cacheline_tag(int64_t addr){
    return addr >> 5;
}

inline int64_t cacheline_addr(int64_t tag, int64_t index){
    return (tag << 5) & (index << 2);
}

// calcula direccion alineada a 32 bytes = 4*sizeof(int64_t)
inline int64_t aligned32_addr(int64_t addr){
    return addr & (~0b11); 
}

class Cache: public Clocked, public MemorySlave, public BusMaster{
private:
    int id;
    CacheLine cache[8]; 
    Bus& bus;

    static int current_id;
    CacheLine& cacheline_check(int64_t addr);
public:
    int invalidations;
    int cache_misses;
    explicit Cache(Bus& bus);
    Cache(Bus& bus, int id);
    int64_t read_request(int64_t addr) override; 
    void write_request(int64_t addr, int64_t value) override;
    
    // métodos de bus master 
    virtual int get_id() override;
    virtual void handle_bus_message(BusMessage_t& msg) override;
};

#endif 