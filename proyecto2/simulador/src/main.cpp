#include <format>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::cout << std::format("Logfile test: {}", 1) << std::endl;
    return 0;
        
}
