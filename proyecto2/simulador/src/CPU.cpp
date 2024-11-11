#include "CPU.hpp"
#include <fstream>
#include <sstream>
#include <format>
#include <regex>
#include <stdexcept>

CPU::CPU(int id, const vector<string> &instructions, MemorySlave *memSlave)
    : id(id), pc(0), registers(4, 0), jump_flag(false), instructionMemory(instructions), memSlave(memSlave)
{
    cout << "CPU initialized with ID: " << id << endl;
}

// Getters (Mostly for testing)

int64_t CPU::getRegister(int regIndex) const
{
    if (regIndex >= 0 && regIndex < registers.size())
    {
        return registers[regIndex];
    }
    throw std::out_of_range("Register index out of bounds");
}

int CPU::getId() const
{
    return id;
}

int CPU::getPC() const
{
    return pc;
}

vector<int64_t> CPU::getRegisters() const
{
    return registers;
}

void CPU::checkZeroCondition(int regIndex)

{
    if (registers[regIndex] != 0)
    {
        jump_flag = true;
    }
    else
    {
        jump_flag = false;
    }
}

// Instructions that can be executed

void CPU::load(int regIndex, int addr)
{
    if (regIndex >= 0 && regIndex < registers.size())
    {
        registers[regIndex] = memSlave->read_request(addr);
        pc++;
    }
}

void CPU::store(int regIndex, int addr)
{
    if (regIndex >= 0 && regIndex < registers.size())
    {
        memSlave->write_request(addr, registers[regIndex]);
        pc++;
    }
}

void CPU::inc(int regIndex)
{
    if (regIndex >= 0 && regIndex < registers.size())
    {
        registers[regIndex]++;
        pc++;
    }
    checkZeroCondition(regIndex);
}

void CPU::dec(int regIndex)
{
    if (regIndex >= 0 && regIndex < registers.size())
    {
        registers[regIndex]--;
        pc++;
    }
    checkZeroCondition(regIndex);
}

void CPU::jnz(int jumpAddress)
{

    if (jump_flag)
    {
        pc = jumpAddress; // Jump to the specified address if jump_flag is true
    }
    else
    {
        pc++;
    }
}

void CPU::mul(int regIndex1, int regIndex2)
{
    if (regIndex1 >= 0 && regIndex1 < registers.size() &&
        regIndex2 >= 0 && regIndex2 < registers.size())
    {
        registers[regIndex1] *= registers[regIndex2];
        pc++;
    }
}

// Shows CPU State (might be used for GUI, called every clk cycle???)
void CPU::showState() const
{
    std::cout << std::format("Estado del CPU {}:\n", id);
    for (int i = 0; i < registers.size(); ++i)
    {
        std::cout << std::format("REG{}: {}\n", i, registers[i]);
    }
    std::cout << std::format("Program Counter (PC): {}\n", pc);
}

// Executes next instruction from the instruction memory
void CPU::executeNextInstruction()
{
    if (pc >= 0 && pc < instructionMemory.size())
    {
        decodeAndExecute(instructionMemory[pc]);
    }
}

// Decodes and executes current instruction
void CPU::decodeAndExecute(const string &instruction)
{
    if (instruction.find("LOAD") == 0)
    {
        int reg = instruction[5] - '0';
        int value = stoi(instruction.substr(7));
        load(reg, value);
    }
    else if (instruction.find("STORE") == 0)
    {
        int reg = instruction[6] - '0';
        int value = stoi(instruction.substr(7));
        store(reg, value);
    }
    else if (instruction.find("INC") == 0)
    {
        int reg = instruction[4] - '0';
        inc(reg);
    }
    else if (instruction.find("DEC") == 0)
    {
        int reg = instruction[4] - '0';
        dec(reg);
    }
    else if (instruction.find("JNZ") == 0)
    {
        int jumpAddress = stoi(instruction.substr(4));
        jnz(jumpAddress);
    }
    else if (instruction.find("MUL") == 0)
    {
        int reg1 = instruction[4] - '0';
        int reg2 = instruction[10] - '0';
        mul(reg1, reg2);
    }
}
