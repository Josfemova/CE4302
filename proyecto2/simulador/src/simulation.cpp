#include "BusInterconnect.hpp"
#include "CPU.hpp"
#include "Cache.hpp"
#include "Clocked.hpp"
#include "Compiler.hpp"
#include "Interfaces.hpp"
#include "SimpleMemory.hpp"
#include "messages.hpp"
#include <string>
#include <thread>
#include <unistd.h>
#include <atomic>

std::atomic_bool stop = false;
std::atomic_bool running = false;
std::atomic_bool pause_simul = false;

Clocked stepper{};

bool ram_path_ready = true;
std::string ram_path{"/home/josfemova-rs/REPOSITORIES/CE4302/proyecto2/workloads/dummy/mem_init_dummy.txt"};
bool instr_path_ready[4]={false, false, false, false};
std::string instr_paths[4];

#define sim_sleep() {usleep(100000);}

Clocked sub_steppers[5];
void execute_pe(CPU &pe)
{
    while (!stop)
    {   
        sim_sleep(); // quantum de la simulacion
        if(!pe.executeNextInstruction()) break;
        if(pause_simul){
            sub_steppers[pe.getId()].step();
        }
    }
}

void update_bus(BusInterconnect &bus)
{
    while (!stop)
    {
        sim_sleep(); // quantum de la simulacion
        bus.update();
        if(pause_simul){
            sub_steppers[4].step();
        }
    }
}

void start_simulation()
{
    for(auto p: instr_path_ready){
        if(!p) return;
    }
    //if (!ram_path_ready) return;
    stop=false;
    running=true;
    
    Compiler compiler;
    std::vector<std::string> instr_pe0 =
        compiler.loadInstructionsFromFile(instr_paths[0]);
    std::vector<std::string> instr_pe1 =
        compiler.loadInstructionsFromFile(instr_paths[1]);
    std::vector<std::string> instr_pe2 =
        compiler.loadInstructionsFromFile(instr_paths[2]);
    std::vector<std::string> instr_pe3 =
        compiler.loadInstructionsFromFile(instr_paths[3]);

    BusInterconnect bus{};
    SimpleMemory main_mem;
    main_mem.load_data(ram_path);
    bus.register_mem_slave(&main_mem, 0, simple_memory_size);

    Cache cache0{bus, 0};
    Cache cache1{bus, 1};
    Cache cache2{bus, 2};
    Cache cache3{bus, 3};

    CPU pe0{0, instr_pe0, &cache0}, pe1{1, instr_pe1, &cache1},
        pe2{2, instr_pe2, &cache2}, pe3{3, instr_pe3, &cache3};

    std::vector<Clocked *> clocked_components = {&bus,    &main_mem, &cache0,
                                                 &cache1, &cache2,   &cache3};

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
        if(pause_simul){
            stepper.step();
            for(auto &x : sub_steppers){
                x.tick();
            }
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
    tbus.join();
    notify::simulation_end(bus, cache0, cache1, cache2, cache3); 
    running=false;
}