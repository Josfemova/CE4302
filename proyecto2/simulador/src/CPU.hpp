#ifndef CPU_HPP
#define CPU_HPP

#include "Interfaces.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

/**
 * @brief Simulates a simple CPU that can load, store, and modify registers.
 *
 * This class represents a basic CPU with 4 registers. It can execute basic assembly-like
 * instructions such as LOAD, STORE, INC, DEC, and JNZ. Instructions can be loaded from a file
 * or a vector of strings, and executed one at a time.
 */
class CPU : public Clocked
{
public:
    /**
     * @brief Constructs a CPU and initializes instruction memory with the provided instructions.
     *
     * @param id The ID of the CPU (useful in multiprocessor simulations).
     * @param instructions A vector of strings representing the instructions to load into the CPU.
     */
    CPU(int id, const vector<string> &instructions);

    /**
     * @brief Loads a value into a specified register.
     *
     * @param regIndex The index of the register to load the value into (0-3).
     * @param value The address of the value to load into the register.
     */
    void load(int regIndex, int value);

    /**
     * @brief Stores the value of a specified register.
     *
     * This function simulates the store operation. Currently, it only prints the value
     * stored in the specified register, as memory handling is not yet implemented.
     *
     * @param regIndex The index of the register whose value is to be stored (0-3).
     * @param addr The address of where the value from the register is to be stored.
     */
    void store(int regIndex, int addr);

    void checkZeroCondition(int regIndex);

    /**
     * @brief Increments the value of the specified register.
     *
     * @param regIndex The index of the register to increment (0-3).
     */
    void inc(int regIndex);

    /**
     * @brief Decrements the value of the specified register.
     *
     * @param regIndex The index of the register to decrement (0-3).
     */
    void dec(int regIndex);

    /**
     * @brief Jumps to a specific instruction if the value in REG1 is equal to one.
     *
     * If the value in REG1 is equal to one, the program counter will be
     * set to the `jumpAddress`, effectively jumping to a different instruction.
     *
     * @param jumpAddress The instruction index to jump to if the value of REG1 is equal to one.
     */
    void jnz(int jumpAddress);

    /**
     * @brief Executes the next instruction in the instruction memory.
     *
     * This function reads the next instruction in the instruction memory, decodes it,
     * and executes it. The program counter is then incremented.
     */
    void executeNextInstruction();

    /**
     * @brief Loads instructions from a vector of strings.
     *
     * Each string in the vector represents a single instruction in the format used by the CPU.
     * This function clears the existing instructions and replaces them with the new ones.
     *
     * @param instructions A vector of strings representing instructions.
     */

    void showState() const;

    // Getters

    /**
     * @brief Returns the unique ID of the CPU.
     *
     * This function retrieves the ID of the CPU instance.
     *
     * @return The unique ID of the CPU.
     */
    int getId() const;

    /**
     * @brief Returns the current value of the program counter.
     *
     * This function retrieves the value of the program counter, which indicates
     * the current instruction being executed.
     *
     * @return The current value of the program counter.
     */
    int getPC() const;

    /**
     * @brief Returns a vector containing the values stored in the registers.
     *
     * This function returns a copy of the current values of the registers.
     *
     * @return A vector of register values.
     */
    vector<int64_t> getRegisters() const;

    /**
     * @brief Returns the value stored in a specific register by index.
     *
     * this functions will return the value stored in a register.
     *
     * @param regIndex The index of the register you want to retrieve the value from
     * @return The value stored in the specified register
     */
    int64_t getRegister(int regIndex) const;

private:
    int id; ///< The unique ID of the CPU.
    int pc; ///< The Program Counter (PC), indicating the current instruction.
    bool jump_flag;
    map<string, int> labels;
    vector<int64_t> registers;        ///< A vector of 4 registers (REG0, REG1, REG2, REG3), each 64 bits.
    vector<string> instructionMemory; ///< The list of instructions loaded into the CPU's instruction memory.

    /**
     * @brief Decodes and executes a single instruction.
     *
     * This function decodes a string instruction and performs the appropriate operation
     * (e.g., LOAD, STORE, INC, DEC, JNZ) on the registers or program counter.
     *
     * @param instruction The instruction to decode and execute.
     */
    void decodeAndExecute(const string &instruction);
};

#endif // CPU_HPP
