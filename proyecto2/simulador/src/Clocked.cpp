#include "Clocked.hpp"

Clocked::Clocked(): ticked{true}{
    
}

void Clocked::tick(){
    ticked = true;
}

void Clocked::step(){
    while(!ticked);
    ticked=false;
}