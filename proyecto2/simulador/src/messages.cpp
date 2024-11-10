#include "messages.hpp"

namespace notify
{
    void cache_update(int cache_id, const CacheLine &line, int index) {}
    void main_mem_write(int64_t addr, int64_t val) {}
    void bus_message(BusMessage_t &msg) {}
}