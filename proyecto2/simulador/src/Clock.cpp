#include "Clock.hpp"

void Clock::tick(){
    ticked = true;
}

void Clock::step(){
    while(!ticked);
    ticked=false;
}