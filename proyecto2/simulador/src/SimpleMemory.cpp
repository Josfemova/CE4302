#include "SimpleMemory.hpp"
#include "messages.hpp"
#include <iostream>
#include <regex>
#include <format>

SimpleMemory::SimpleMemory(): Clocked(){}

int64_t SimpleMemory::read_request(int64_t addr) {
    int index = addr;
    this->step(); // flecha de entrada
    if (index > (simple_memory_size-1)){
        throw 22;
    }else{
        this->step(); // flecha de salida
        return this->memory[index];
    }
} 

void SimpleMemory::write_request(int64_t addr, int64_t value) {
    int index = addr;
    this->step(); // flecha de entrada
    if (index > (simple_memory_size-1)){
        throw 22;
    }else{
        notify::update_memory(addr, value);
        this->memory[index] = value;
    }
}

void SimpleMemory::load_data(const std::string &filename){
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << std::format("Error: No se pudo abrir el archivo {}\n", filename);
        throw 33;
    }
    // cada linea es un valor
    std::string line;
    int index = 0;
    while (std::getline(file, line)){
        if(index > (simple_memory_size-1)){
            std::cerr << std::format("Demasiados valores en inicializador de mem\n", filename);
            throw 34;
        }
        this->memory[index]=std::stol(line, NULL , 0);
        notify::update_memory(index, this->memory[index]);
        index++;
    }
    while(index < simple_memory_size){
        this->memory[index]=0;
        notify::update_memory(index, this->memory[index]);
        index++;
    }
    file.close();

}