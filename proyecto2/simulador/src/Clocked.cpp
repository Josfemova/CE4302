#include "Clocked.hpp"
#include <chrono>
#include <thread>

Clocked::Clocked() : ticked{true}, shutdown_flag{false}{}

void Clocked::tick() { ticked = true; }

void Clocked::step()
{
    while ((!ticked) && (!shutdown_flag)){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    ticked = false;
}

void Clocked::shutdown(){
    this->shutdown_flag = true;
}