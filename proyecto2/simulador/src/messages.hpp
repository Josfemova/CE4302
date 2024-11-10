#include <cstdint>
#include "Cache.hpp"

namespace notify{

//line me da state, tag y data
void cache_update(int cache_id, const CacheLine& line, int index);
void main_mem_write(int64_t addr, int64_t val);
void bus_message(BusMessage_t& msg);

}