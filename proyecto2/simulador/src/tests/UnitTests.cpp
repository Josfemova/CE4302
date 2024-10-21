#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
#include "../CPU.hpp"
#include "../Compiler.hpp"

#include <fstream>
#include <iostream>

int testNumber = 0;
int totalTests = 0;

void createMockInstructionsFile(const std::string &filename, int caseNumber)
{
    std::ofstream file(filename);
    if (file.is_open())
    {
        switch (caseNumber)
        {
        case 1: // Valid Instructions
            file << "LOAD REG1 10\n";
            file << "INC REG1\n";
            file << "STORE REG1 20\n";
            file << "this_is_a_label:\n";
            file << "LOAD REG1 10\n";
            file << "INC REG1\n";
            file << "STORE REG1 20\n";
            file << "JNZ [this_is_a_label]\n";
            break;

        case 2: // Invalid Label
            file << "LOAD REG1 10\n";
            file << "INC REG1\n";
            file << "JNZ [non_existent_label]\n"; // Invalid label reference
            break;

        case 3: // Invalid Instruction
            file << "LOAD REG1 10\n";
            file << "INC\n"; // Invalid instruction
            file << "STORE REG2 20\n";
            break;

        default:
            std::cerr << "Invalid case number provided." << std::endl;
            return;
        }

        file.close();
    }
    else
    {
        std::cerr << "Unable to create the mock instructions file." << std::endl;
    }
}

// --------------------------------- Compiler Tests START ---------------------------------

// Función para comparar dos vectores de cadenas
bool areVectorsEqual(const std::vector<std::string> &vec1, const std::vector<std::string> &vec2)
{
    if (vec1.size() != vec2.size())
    {
        return false;
    }
    for (size_t i = 0; i < vec1.size(); ++i)
    {
        if (vec1[i] != vec2[i])
        {
            return false;
        }
    }
    return true;
}

void testCompiler(string mockFileName)
{
    totalTests++;
    Compiler compiler;

    // Cargar instrucciones
    std::vector<std::string> instructions = compiler.loadInstructionsFromFile(mockFileName);

    // Vector de instrucciones esperadas
    std::vector<std::string> expectedInstructions = {
        "LOAD 1 10",
        "INC 1",
        "STORE 1 20",
        "LOAD 1 10",
        "INC 1",
        "STORE 1 20",
        "JNZ 3" // Ajustar según la posición de la etiqueta o el valor que se espera
    };

    // Verificar que las instrucciones cargadas son las esperadas
    if (areVectorsEqual(instructions, expectedInstructions))
    {
        std::cout << std::format("Test {}: Instructions match expected output: PASSED\n", testNumber);
        testNumber++;
    }
}

void testInvalidLabel(string mockFileName)
{
    totalTests++;
    Compiler compiler;

    // Cargar instrucciones
    std::vector<std::string> instructions = compiler.loadInstructionsFromFile(mockFileName);

    // Verificar que las instrucciones están vacías debido al error
    if (instructions.empty())
    {
        std::cout << std::format("Test {}: Check for invalid label: PASSED\n", testNumber);
        testNumber++;
    }
    else
    {
        std::cout << "Test for invalid label failed: Instructions loaded unexpectedly.\n";
    }
}

void testInvalidInstruction(string mockFileName)
{
    totalTests++;
    Compiler compiler;

    // Cargar instrucciones
    std::vector<std::string> instructions = compiler.loadInstructionsFromFile(mockFileName);

    // Verificar que las instrucciones están vacías debido al error
    if (instructions.empty())
    {
        std::cout << std::format("Test {}: Check for invalid instructions: PASSED\n", testNumber);
        testNumber++;
    }
    else
    {
        std::cout << "Test for invalid instruction failed: Instructions loaded unexpectedly.\n";
    }
}

// --------------------------------- Compiler Tests END ---------------------------------

// --------------------------------- CPU Tests START ---------------------------------

// Unit test for increment function
void testCPUInc(int cpu_id, const std::string &instructions_file)
{
    totalTests++;
    Compiler compiler;

    std::vector<std::string> instructions = compiler.loadInstructionsFromFile(instructions_file);
    CPU cpu(cpu_id, instructions);

    assert(cpu.getRegisters()[0] == 0);
    assert(cpu.getRegisters()[1] == 0);
    assert(cpu.getRegisters()[2] == 0);
    assert(cpu.getRegisters()[3] == 0);

    cpu.inc(0);
    assert(cpu.getRegisters()[0] == 1);

    cpu.inc(1);
    assert(cpu.getRegisters()[1] == 1);

    cpu.inc(0);
    assert(cpu.getRegisters()[0] == 2);

    std::cout << std::format("Test {}: CPU Increment function: PASSED\n", testNumber);
    testNumber++;
}

// Unit test for decrement function
void testCPUDec(int cpu_id, const std::string &instructions_file)
{
    totalTests++;
    Compiler compiler;

    std::vector<std::string> instructions = compiler.loadInstructionsFromFile(instructions_file);
    CPU cpu(cpu_id, instructions);

    cpu.load(0, 2);
    cpu.load(1, 3);

    cpu.dec(0);
    assert(cpu.getRegisters()[0] == 1);

    cpu.dec(1);
    assert(cpu.getRegisters()[1] == 2);

    cpu.dec(1);
    assert(cpu.getRegisters()[1] == 1);

    std::cout << std::format("Test {}: CPU Decrement function: PASSED\n", testNumber);
    testNumber++;
}

// --------------------------------- CPU Tests END ---------------------------------

int main()
{

    // Create instructions mock file for tests
    const std::string mock_instructions_file = "tests/mock_instructions.s";
    const std::string invalid_label = "tests/invalid_label.s";
    const std::string invalid_instruction = "tests/invalid_instruction.s";

    // Create mock instruction files
    createMockInstructionsFile(mock_instructions_file, 1);
    createMockInstructionsFile(invalid_label, 2);
    createMockInstructionsFile(invalid_instruction, 3);

    // Compiler test functions
    testCompiler(mock_instructions_file);
    testInvalidLabel(invalid_label);
    testInvalidInstruction(invalid_instruction);

    // CPU test functions
    testCPUInc(1, mock_instructions_file);
    testCPUDec(1, mock_instructions_file);

    std::cout << std::format("Pruebas aprobadas: {}/{}\n", testNumber, totalTests);

    return 0;
}
