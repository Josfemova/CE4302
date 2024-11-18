#include "messages.hpp"
#include "Server.hpp"
#include <atomic>
#include <format>
#include <iostream>
#include <mutex>
#include <regex>
#include <thread>

std::mutex server_mutex;

static void send_message_gui(const std::string &msg)
{
    server_mutex.lock();
    sendMessageToClient(msg);
    server_mutex.unlock();
}

namespace notify
{
    void update_register(int pe_id, int reg_id, int64_t data)
    {
        send_message_gui(
            std::format("update_register,{},{},[{}]", pe_id, reg_id, data));
    }
    void update_cache(int cache_id, const CacheLine &line, int index)
    {
        char state = mesi_state_to_char(line.state);
        send_message_gui(std::format(
            "update_block,{},{},[State:{} Tag:{} offset:{} "
            "values: - - - - - - - - - {:04} | {:04} | {:04} | {:04}]",
            cache_id, index, state, line.tag, index, line.data[0], line.data[1],
            line.data[2], line.data[3]));
    }
    void update_memory(int64_t addr, int64_t val)
    {
        send_message_gui(std::format("update_memory,0x{:04X},[{}]", addr, val));
    }
    void interconnect_event(BusMessage_t &msg)
    {
        send_message_gui(std::format(
            "event,0,[>>>bus-req ={}= req_id:{} addr:{}]",
            bus_message_type_map[msg.type], msg.master_id, msg.address));
    }
    void flush_opt(BusMessage_t &msg, int responder_id)
    {
        send_message_gui(std::format(
            "event,0,[<<<bus-resp ={}= req_id {} resp_id :{} addr:{}]",
            bus_message_type_map[msg.type], msg.master_id, responder_id,
            msg.address));
    }

    void bus_interconnect_update(BusInterconnect &bus)
    {
        send_message_gui(
            std::format("update_interconnect,1,0,[{}]", bus.read_reqs));
        send_message_gui(
            std::format("update_interconnect,2,0,[{}]", bus.read_resp));
        send_message_gui(
            std::format("update_interconnect,3,0,[{}]", bus.write_reqs));
        send_message_gui(
            std::format("update_interconnect,4,0,[{}]", bus.write_reqs));
        // datos transmitidos a pe
        send_message_gui(std::format("update_interconnect,5,0,[{} bytes]",
                                     bus.pe_data_tx[0]));
        send_message_gui(std::format("update_interconnect,5,1,[{} bytes]",
                                     bus.pe_data_tx[1]));
        send_message_gui(std::format("update_interconnect,5,2,[{} bytes]",
                                     bus.pe_data_tx[2]));
        send_message_gui(std::format("update_interconnect,5,3,[{} bytes]",
                                     bus.pe_data_tx[3]));
    }
    void simulation_end(BusInterconnect &bus, uint64_t ticks, const Cache &c0,
                        const Cache &c1, const Cache &c2, const Cache &c3)
    {
        send_message_gui(
            std::format("stats,cache_misses,[cache0 =>>|{}| "
                        "\tcache1 =>>|{}| \tcache2 =>>|{}| \tcache3 =>>|{}|]",
                        c0.cache_misses, c1.cache_misses, c2.cache_misses,
                        c3.cache_misses));
        send_message_gui(
            std::format("stats,cache_invalidates,[cache0=>>|{}| "
                        "\tcache1 =>>|{}| \tcache2 =>>|{}| \tcache3 =>>:|{}|]",
                        c0.invalidations, c1.invalidations, c2.invalidations,
                        c3.invalidations));
        send_message_gui(
            std::format("stats,bus_invalidations,[{}]", bus.invalidations));
        send_message_gui(std::format(
            "stats,main_mem,[writes=>>|{}|  reads=>>|{}| tx_bytes=>>|{}| rx_bytes=>>|{}|]",
            bus.main_mem_writes, bus.main_mem_reads, bus.main_mem_writes * 32,
            bus.main_mem_reads * 32));
        send_message_gui(std::format("stats,system_ticks,[{}]", ticks));
        bus_interconnect_update(bus);
    }

}

extern std::atomic_bool stop;
extern std::atomic_bool running;
extern std::atomic_bool pause_simul;

extern Clocked stepper;

extern bool ram_path_ready;
extern std::string ram_path;
extern bool instr_path_ready[4];
extern std::string instr_paths[4];
extern void start_simulation();

void handle_command(const std::string &command)
{
    std::regex load_pe_regex{R"(@load_pe,([0-9]+),\[(.+)\]\$)"};
    std::regex load_mem_regex{R"(@lmem,([0-9]+),\[(.+)\]\$)"};
    std::smatch match;
    if (command == "@play,0,[]$")
    {
        std::cout << "play \n";
        if (running)
        {
            // está en modo stepping
            pause_simul = false;
            stepper.tick();
        }
        else
        {
            // no ha iniciado
            std::thread run_sim(start_simulation);
            run_sim.detach();
        }
    }
    else if (command == "@pause,0,[]$")
    {
        std::cout << "pause \n";
        pause_simul = true;
    }
    else if (command == "@step,0,[]$")
    {
        std::cout << "step \n";
        stepper.tick();
    }
    else if (command == "@stop,0,[]$")
    {
        std::cout << "stop \n";
        stop = true;
    }
    else if (std::regex_match(command, match, load_pe_regex))
    {
        int pe_id = std::stoi(match[1]);
        std::string path = match[2];
        instr_path_ready[pe_id] = true;
        instr_paths[pe_id] = path;
        std::cout << std::format("load_pe id:{} path:{}\n", pe_id, path);
    }
    else if (std::regex_match(command, match, load_mem_regex))
    {
        int mem_id = std::stoi(match[1]);
        std::string path = match[2];
        ram_path_ready = true;
        ram_path = path;
    }
}
