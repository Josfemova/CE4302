#include <format>
#include <iostream>
#include <string>
#include "Cache.hpp"
#include "BusInterconnect.hpp"

// Forward declaration of the start_server function in Server.cpp
void start_server();

int main(int argc, char *argv[])
{
    std::cout << std::format("Logfile test: {}", 1) << std::endl;

    // Start the WebSocket server
    start_server();

    return 0;
}
