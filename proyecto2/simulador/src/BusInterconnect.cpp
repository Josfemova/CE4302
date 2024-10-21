#include "BusInterconnect.hpp"

void BusInterconnect::register_mem_slave(MemorySlave* mem_slave, word start_addr, word end_addr) {
    MemorySlaveCard card = {
        .ptr=mem_slave,
        .start_addr = start_addr,
        .end_addr = end_addr
    };
    this->mem_slaves.push_back(card);
}

void BusInterconnect::register_mem_master(MemoryMaster* mem_master){
    MemoryMasterCard card = {
        .ptr = mem_master,
        .read_requests=0,
        .write_requests=0
    };
    this->mem_masters.push_back(card);
}

word BusInterconnect::service_read_request(word addr){
    // enviar mensaje de que se solicita un valor. 

    // Log de que se puso el mensaje de read en el bus

}

void BusInterconnect::service_write_request(word addr, word value){
    // enviar mensaje de invalidación a todos los masters registrados 
    clock->step();
    //
}

std::vector<word> BusInterconnect::read_request(word addr, int burst_size, int from_ID){
    // averiguar si es el turno del cliente
}

void BusInterconnect::write_request(word addr, word value, int from_ID){
    // averiguar si es el turno del cliente

}

BusInterconnect::BusInterconnect(Clock* clock): clock{clock} {

}
