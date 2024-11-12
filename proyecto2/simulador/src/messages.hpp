#include "Cache.hpp"
#include <cstdint>

namespace notify
{

    void update_register(int pe_id, int reg_id, int64_t data);
    void update_cache(int cache_id, const CacheLine &line, int index);
    void update_memory(int64_t addr, int64_t val);
    void interconnect_event(BusMessage_t &msg);
    void flush_opt(BusMessage_t &msg, int responder_id);
}

void handle_command(const std::string &command);