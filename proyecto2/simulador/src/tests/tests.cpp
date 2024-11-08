#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "../CPU.hpp"
#include "../Compiler.hpp"

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

        case 4: // Instructions including JNZ with a label (simple case)

            // Setting REG0 to 10 by incrementing it 10 times
            for (int i = 0; i < 10; ++i)
            {
                file << "INC REG0\n";
            }

            // Increment and Decrement loop as per the original example
            file << "INC REG0\n";  // Increment REG0 -> 11
            file << "JNZ [end]\n"; // Jump to 'end' if jump flag
            file << "INC REG1\n";  // This will not be executed because of the jump
            file << "end:\n";      // Label 'end'
            file << "INC REG0\n";  // Increment REG0 (this will happen after jump, REG0 = 12)
            break;

        case 5: // Extensive use of JNZ with two labels, keeping REG1 as 0 or 1
            // Set REG0 to 10
            for (int i = 0; i < 10; ++i)
            {
                file << "INC REG0\n";
            }

            // Set REG1 to 1
            file << "INC REG1\n";

            // Set REG2 to 0 (already initialized to 0, so no INC needed)

            // Perform some operations
            file << "INC REG0\n";     // Increment REG0 (REG0 = 11)
            file << "DEC REG1\n";     // Decrement REG1 (REG1 = 0)
            file << "JNZ [label1]\n"; // Jump to label1 if REG1 != 0

            file << "INC REG2\n"; // This will be executed because REG1 was 0 and the jump did not happen

            file << "label1:\n";  // Label1
            file << "INC REG2\n"; // Increment REG2 (REG2 = 1)

            // Now, we reset REG1 to 1 (we make sure it's 1 for the second jump)
            file << "INC REG1\n"; // Set REG1 back to 1

            file << "DEC REG0\n";     // Decrement REG0 (REG0 = 10)
            file << "JNZ [label2]\n"; // Jump to label2 if REG0 != 0

            file << "STORE REG1 10\n"; // This will not be executed because of the jump

            file << "label2:\n";  // Label2
            file << "INC REG2\n"; // Increment REG2 (REG2 = 2)

            // Final instruction
            file << "INC REG0\n"; // Increment REG0 (REG0 = 11)
            break;

        case 6:                           // Loop Implementation - Summing REG3 until 10 with REG1 controlling the jumps
            file << "INC REG3\n";         // Initialize REG3 to 1 (instead of loading 0)
            file << "INC REG3\n";         // Start counting up from 1 to 10
            file << "INC REG3\n";         // Increment REG3 to 3
            file << "INC REG3\n";         // Increment REG3 to 4
            file << "INC REG3\n";         // Increment REG3 to 5
            file << "INC REG3\n";         // Increment REG3 to 6
            file << "INC REG3\n";         // Increment REG3 to 7
            file << "INC REG3\n";         // Increment REG3 to 8
            file << "INC REG3\n";         // Increment REG3 to 9
            file << "INC REG3\n";         // Increment REG3 to 10
            file << "loop_start:\n";      // Label to mark the start of the loop
            file << "DEC REG3\n";         // Increment REG3 (REG3++)
            file << "JNZ [loop_start]\n"; // Jump back to loop_start if REG1 != 0

            // Once REG3 reaches 0, the loop ends, and the next instruction executes
            file << "INC REG3\n"; // Final increment after loop exits
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
        std::cout << "------------" << std::endl;
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
        std::cout << "------------" << std::endl;
        testNumber++;
    }
    else
    {
        std::cout << "Test for invalid label failed: Instructions loaded unexpectedly.\n";
        std::cout << "------------" << std::endl;
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
        std::cout << "------------" << std::endl;
        testNumber++;
    }
    else
    {
        std::cout << "Test for invalid instruction failed: Instructions loaded unexpectedly.\n";
        std::cout << "------------" << std::endl;
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
    std::cout << "------------" << std::endl;
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
    std::cout << "------------" << std::endl;
    testNumber++;
}

// --------------------------------- CPU Tests END ---------------------------------

// --------------------------------- CPU Tests using Assembly START ---------------------------------

void testCPUWithFileJump(int cpu_id, string mockFileName)
{
    totalTests++;
    Compiler compiler;

    std::vector<std::string> instructions = compiler.loadInstructionsFromFile(mockFileName);

    CPU cpu(cpu_id, instructions);

    // Step 1: Set up initial values by incrementing registers

    // Execute first set of instructions: Setting REG0 to 10
    for (int i = 0; i < 10; ++i)
    {
        cpu.executeNextInstruction();
    }
    assert(cpu.getRegister(0) == 10); // REG0 should be 10

    // Step 2: Verify the rest of the instructions

    // Execute third instruction: INC REG0
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 11); // REG0 should be 11

    // Execute fourth instruction: JNZ [end] (should jump if jump_flag is true)
    cpu.executeNextInstruction();
    // Jump happens because REG0 != 0, so jump_flag is set to true
    assert(cpu.getPC() == 13); // PC should jump to the instruction after INC REG1 (label 'end')

    // Execute the next instruction: INC REG0 (this will happen after the jump)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 12); // REG0 should now be 12

    // Final check
    std::cout << std::format("Test {}: CPU ExecuteNextInstructionJump function: PASSED\n", testNumber);
    std::cout << "------------" << std::endl;
    testNumber++;
}

void testCPUWithFileJumpAndNoJump(int cpu_id, const std::string &mockFileName)
{
    totalTests++;
    Compiler compiler;

    // Load the instructions from the file
    std::vector<std::string> instructions = compiler.loadInstructionsFromFile(mockFileName);

    // Create a CPU instance with the instructions
    CPU cpu(cpu_id, instructions);

    // Step 1: Execute instructions and check register values after each one

    // Set REG0 to 10 with INCs
    for (int i = 0; i < 10; ++i)
    {
        cpu.executeNextInstruction();
    }
    assert(cpu.getRegister(0) == 10); // REG0 should be 10

    // Set REG1 to 1 with INC
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 1); // REG1 should be 1

    // No INCs needed for REG2 as it’s already 0

    // Execute next instruction: INC REG0
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 11); // REG0 should be 11

    // Execute next instruction: DEC REG1 (REG1 becomes 0)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 0); // REG1 should be 0

    // Execute JNZ [label1] (REG1 == 0, so it should NOT jump)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 14); // PC should NOT jump, continue to the next instruction (INC REG2)

    // Execute INC REG2 (This should execute because JNZ did NOT jump)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(2) == 1); // REG2 should be 1

    // Execute label1 INC REG2
    cpu.executeNextInstruction();
    assert(cpu.getRegister(2) == 2); // REG2 should be 2

    // Execute INC REG1 (Reset REG1 to 1)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 1); // REG1 should be 1

    // Execute DEC REG0
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 10); // REG0 should be 10

    // Execute JNZ [label2] (REG0 != 0, so it should jump)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 20); // PC should jump to label2

    // Execute INC REG2 in label2
    cpu.executeNextInstruction();
    assert(cpu.getRegister(2) == 3); // REG2 should be 3

    // Execute final INC REG0 (This happens after jump)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 11); // REG0 should be 11

    // Final check
    std::cout << std::format("Test {}: CPU ExecuteNextInstruction Jump/NoJump: PASSED\n", testNumber);
    std::cout << "------------" << std::endl;
    testNumber++;
}

void testCPUWithFileLoop(int cpu_id, const std::string &mockFileName)
{
    totalTests++;
    Compiler compiler;

    std::vector<std::string> instructions = compiler.loadInstructionsFromFile(mockFileName);

    CPU cpu(cpu_id, instructions);

    // Step 1: Set up initial values (Initialize REG3)
    for (int i = 0; i < 10; ++i)
    {
        cpu.executeNextInstruction(); // Increment REG3 10 times
    }
    assert(cpu.getRegister(3) == 10); // REG3 should be 10 after initialization

    // Step 2: Execute loop and decrement REG3
    // Execute the loop: DEC REG3 until REG3 reaches 0
    cpu.executeNextInstruction();    // First DEC REG3
    assert(cpu.getRegister(3) == 9); // REG3 should be 9

    // The loop will continue until REG3 reaches 0
    while (cpu.getRegister(3) != 0)
    {
        cpu.executeNextInstruction(); // Continue executing the loop (DEC REG3 + JNZ)
    }

    // After exiting the loop, REG3 should be 0
    assert(cpu.getRegister(3) == 0); // REG3 should be 0 after exiting the loop

    // jump instruction
    cpu.executeNextInstruction();

    // Step 3: Verify final increment after the loop ends
    cpu.executeNextInstruction();    // INC REG3 after the loop
    assert(cpu.getRegister(3) == 1); // REG3 should now be 1 after increment

    // Final check
    std::cout << std::format("Test {}: CPU Loop Test (Summing REG3) PASSED\n", testNumber);
    std::cout << "------------" << std::endl;
    testNumber++;
}

// --------------------------------- CPU Tests using Assembly END ---------------------------------

int main()
{

    // Create instructions mock file for tests
    const std::string mock_instructions_file = "tests/mock_instructions.s";
    const std::string invalid_label = "tests/invalid_label.s";
    const std::string invalid_instruction = "tests/invalid_instruction.s";
    const std::string mock_instructions_file_2 = "tests/mock_instructions_2.s";
    const std::string mock_instructions_file_3 = "tests/mock_instructions_3.s";
    const std::string mock_instructions_file_4 = "tests/mock_instructions_4.s";

    // Create mock instruction files
    createMockInstructionsFile(mock_instructions_file, 1);
    createMockInstructionsFile(invalid_label, 2);
    createMockInstructionsFile(invalid_instruction, 3);
    createMockInstructionsFile(mock_instructions_file_2, 4);
    createMockInstructionsFile(mock_instructions_file_3, 5);
    createMockInstructionsFile(mock_instructions_file_4, 6);

    // Compiler test functions
    testCompiler(mock_instructions_file);
    testInvalidLabel(invalid_label);
    testInvalidInstruction(invalid_instruction);

    // CPU test functions
    testCPUInc(1, mock_instructions_file);
    testCPUDec(1, mock_instructions_file);

    // CPU assembly tests
    testCPUWithFileJump(1, mock_instructions_file_2);
    testCPUWithFileJumpAndNoJump(1, mock_instructions_file_3);
    testCPUWithFileLoop(1, mock_instructions_file_4);

    std::cout << std::format("Pruebas aprobadas: {}/{}\n", testNumber, totalTests);

    return 0;
}
