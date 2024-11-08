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

            // Set REG0 to 10 by incrementing it 10 times
            for (int i = 0; i < 10; ++i)
            {
                file << "INC REG0\n"; // Increment REG0 (REG0 will become 10)
            }

            // Increment REG0 once more, making it 11
            file << "INC REG0\n"; // Increment REG0 -> REG0 = 11

            // Check if REG0 is non-zero and jump to 'end' if true
            file << "JNZ [end]\n"; // If REG0 != 0, jump to 'end'

            // This line will be skipped because of the jump
            file << "INC REG1\n"; // This will NOT execute due to jump

            // Define the 'end' label where the code resumes after the jump
            file << "end:\n"; // Label 'end'

            // Increment REG0 after the jump, which will now make REG0 = 12
            file << "INC REG0\n"; // Increment REG0 (REG0 = 12)
            break;

        case 5: // Extensive use of JNZ with two labels, keeping REG1 as 0 or 1

            // Set REG0 to 10 by incrementing it 10 times
            for (int i = 0; i < 10; ++i)
            {
                file << "INC REG0\n"; // Increment REG0 (REG0 = 10 after 10 increments)
            }

            // Set REG1 to 1
            file << "INC REG1\n"; // Increment REG1 (REG1 = 1)

            // Set REG2 to 0 (no need to increment as it is initialized to 0)

            // Perform some operations:
            file << "INC REG0\n"; // Increment REG0 (REG0 = 11)
            file << "DEC REG1\n"; // Decrement REG1 (REG1 = 0)

            // Jump to 'label1' if REG1 is not zero
            file << "JNZ [label1]\n"; // Jump to 'label1' if REG1 != 0

            // This instruction will execute since REG1 was 0
            file << "INC REG2\n"; // Increment REG2 (REG2 = 1)

            // Label1 will only be reached if REG1 was non-zero
            file << "label1:\n"; // Label1

            // Increment REG2 as part of label1
            file << "INC REG2\n"; // Increment REG2 (REG2 = 2)

            // Set REG1 back to 1 for the next jump scenario
            file << "INC REG1\n"; // Increment REG1 (REG1 = 1)

            // Decrement REG0 and check if it's non-zero to jump to label2
            file << "DEC REG0\n";     // Decrement REG0 (REG0 = 10)
            file << "JNZ [label2]\n"; // Jump to label2 if REG0 != 0

            // This will not be executed because REG0 is not zero and the jump happens
            file << "STORE REG1 10\n"; // This will be skipped because of the jump

            // Label2 where the code jumps to after REG0 is decremented
            file << "label2:\n"; // Label2

            // Increment REG2 as part of label2
            file << "INC REG2\n"; // Increment REG2 (REG2 = 3)

            // Final increment of REG0 (REG0 = 11)
            file << "INC REG0\n"; // Increment REG0 (REG0 = 11)
            break;

        case 6: // Loop Implementation - Summing REG3 until 10 with REG1 controlling the jumps

            // Initialize REG3 to 1 by incrementing it twice (REG3 = 2)
            file << "INC REG3\n"; // Increment REG3 (REG3 = 1)
            file << "INC REG3\n"; // Increment REG3 (REG3 = 2)

            // Continue incrementing REG3 until it reaches 10
            file << "INC REG3\n"; // Increment REG3 (REG3 = 3)
            file << "INC REG3\n"; // Increment REG3 (REG3 = 4)
            file << "INC REG3\n"; // Increment REG3 (REG3 = 5)
            file << "INC REG3\n"; // Increment REG3 (REG3 = 6)
            file << "INC REG3\n"; // Increment REG3 (REG3 = 7)
            file << "INC REG3\n"; // Increment REG3 (REG3 = 8)
            file << "INC REG3\n"; // Increment REG3 (REG3 = 9)
            file << "INC REG3\n"; // Increment REG3 (REG3 = 10)

            // Define the start of the loop
            file << "loop_start:\n"; // Label marking the start of the loop

            // Decrement REG3 (this will be done in the loop)
            file << "DEC REG3\n"; // Decrement REG3 (REG3 will be decremented)

            // If REG3 is not zero, jump back to 'loop_start' to continue looping
            file << "JNZ [loop_start]\n"; // Jump back to loop_start if REG3 != 0

            // Once REG3 reaches 0, the loop ends and the code below executes
            file << "INC REG3\n"; // Increment REG3 (REG3 = 1, after the loop ends)
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
