#include "Cache.hpp"
int Cache::current_id = 0;
Cache::Cache(Bus& bus)
    : id{this->current_id++}, cache{}, invalidations{}, cache_misses{}, bus{bus} {}

int64_t Cache::read_request(int64_t addr) {  // ojo, addr cuenta por byte
  int64_t tag = addr >> 5;
  int offset = (addr >> 3) & 0b11;  // que multiplo de 8 es
  if ((addr % 8) != 0) {
    throw 20;  // dirección debe ser alineada a 8 bytes
  }
  for (const auto& line : this->cache) {
    if (line.tag == tag) {
      // cache hit
      return line.data[offset];
    }
  }

  // cache miss
  // TODO encontrar slot vacío de caché o reemplazar un slot
  auto slot = &this->cache[0];

  BusMessage_t request = {
    .type = BusMessageType::BusRd,
    .address = addr & (~0x1F),
    .data = slot->data,
    .master_id = this->id,
    .completed = false
  };
  this->bus.bus_request(request);

  // Linea de caché ya fue cargada, ahora a retornar el valor 
  return slot->data[offset];
}

void Cache::write_request(int64_t addr, int64_t value) {}

int Cache::get_id(){
    return this->id;
}

void Cache::handle_bus_message(BusMessage_t& msg){
}