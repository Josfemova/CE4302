#include "Clocked.hpp"

Clocked::Clocked() : ticked{true}, shutdown_flag{false}{}

void Clocked::tick() { ticked = true; }

void Clocked::step()
{
    while ((!ticked) && (!shutdown_flag))
        ;
    ticked = false;
}

void Clocked::shutdown(){
    this->shutdown_flag = true;
}