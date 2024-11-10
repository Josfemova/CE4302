#include "BusInterconnect.hpp"
#include "CPU.hpp"
#include "Cache.hpp"
#include "Clocked.hpp"
#include "Compiler.hpp"
#include "Interfaces.hpp"
#include "SimpleMemory.hpp"
#include <string>
#include <thread>
#include <unistd.h>

bool stop = false;
#define sim_sleep() sleep(1)

void execute_pe(CPU &pe)
{
    while (!stop)
    {
        sim_sleep(); // quantum de la simulacion
        pe.executeNextInstruction();
    }
}
void update_bus(BusInterconnect &bus)
{
    while (!stop)
    {
        sim_sleep(); // quantum de la simulacion
        bus.update();
    }
}

void start_simulation(const std::string &pe0_file, const std::string &pe1_file,
                      const std::string &pe2_file, const std::string &pe3_file,
                      const std::string &main_mem_file)
{
    Compiler compiler;
    std::vector<std::string> instr_pe0 =
        compiler.loadInstructionsFromFile(pe0_file);
    std::vector<std::string> instr_pe1 =
        compiler.loadInstructionsFromFile(pe1_file);
    std::vector<std::string> instr_pe2 =
        compiler.loadInstructionsFromFile(pe2_file);
    std::vector<std::string> instr_pe3 =
        compiler.loadInstructionsFromFile(pe3_file);

    CPU pe0{0, instr_pe0}, pe1{1, instr_pe1}, pe2{2, instr_pe2},
        pe3{3, instr_pe3};

    BusInterconnect bus{};
    SimpleMemory main_mem;
    main_mem.load_data(main_mem_file);
    bus.register_mem_slave(&main_mem, 0, simple_memory_size);

    Cache cache0{bus, 0};
    Cache cache1{bus, 1};
    Cache cache2{bus, 2};
    Cache cache3{bus, 3};

    std::vector<Clocked *> clocked_components = {
        &pe0,      &pe1,    &pe2,    &pe3,    &bus,
        &main_mem, &cache0, &cache1, &cache2, &cache3};

    std::thread t0(execute_pe, std::ref(pe0));
    std::thread t1(execute_pe, std::ref(pe1));
    std::thread t2(execute_pe, std::ref(pe2));
    std::thread t3(execute_pe, std::ref(pe3));
    std::thread tbus(update_bus, std::ref(bus));

    while (!stop)
    {
        sim_sleep(); // quantum de la simulacion
        for (auto c : clocked_components)
        {
            c->tick();
        }
    }
    for (auto c : clocked_components)
    {
        c->shutdown();
    }
    bus.abort_exec();
    t0.join();
    t1.join();
    t2.join();
    t3.join();
}