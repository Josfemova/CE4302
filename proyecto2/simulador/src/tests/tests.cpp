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

        case 4:                       // Instructions including JNZ with a label (simple case)
            file << "LOAD REG0 10\n"; // Load 10 into REG0
            file << "LOAD REG1 20\n"; // Load 20 into REG1
            file << "INC REG0\n";     // Increment REG0 (REG0 = 11)
            file << "DEC REG1\n";     // Decrement REG1 (REG1 = 19)
            file << "JNZ [end]\n";    // Jump to 'end' if REG1 != 0
            file << "INC REG1\n";     // This will not be executed because of the jump
            file << "end:\n";         // Label 'end'
            file << "INC REG0\n";     // Increment REG0 (this will happen after jump, REG0 = 12)
            break;

        case 5:                       // Extensive use of JNZ with two labels, keeping REG1 as 0 or 1
            file << "LOAD REG0 10\n"; // Load 10 into REG0
            file << "LOAD REG1 1\n";  // Load 1 into REG1 (we will keep REG1 as 1 or 0)
            file << "LOAD REG2 0\n";  // Load 0 into REG2

            // Perform some operations
            file << "INC REG0\n";     // Increment REG0 (REG0 = 11)
            file << "DEC REG1\n";     // Decrement REG1 (REG1 = 0)
            file << "JNZ [label1]\n"; // Jump to label1 if REG1 != 0 (it will NOT jump because REG1 == 0)

            file << "INC REG2\n"; // This will be executed because REG1 was 0 and the jump did not happen

            file << "label1:\n";  // Label1
            file << "INC REG2\n"; // Increment REG2 (REG2 = 1)

            // Now, we reset REG1 to 1 (we make sure it's 1 for the second jump)
            file << "INC REG1\n"; // Set REG1 back to 1

            file << "DEC REG0\n";     // Decrement REG0 (REG0 = 10)
            file << "JNZ [label2]\n"; // Jump to label2 if REG0 != 0 (it will jump because REG0 != 0)

            file << "STORE REG1 10\n"; // This will not be executed because of the jump

            file << "label2:\n";  // Label2
            file << "INC REG2\n"; // Increment REG2 (REG2 = 2)

            // Final instruction
            file << "INC REG0\n"; // Increment REG0 (REG0 = 11)
            break;

        case 6:                       // Loop Implementation - Summing REG3 until 10 with REG1 controlling the jumps
            file << "LOAD REG3 0\n";  // Load 0 into REG3 (the counter)
            file << "LOAD REG1 10\n"; // Load 10 into REG1 (this will control the loop)

            // Start of the loop
            file << "loop_start:\n";      // Label to mark the start of the loop
            file << "INC REG3\n";         // Increment REG3 (REG3++)
            file << "DEC REG1\n";         // Decrement REG1 (REG1--)
            file << "JNZ [loop_start]\n"; // Jump back to loop_start if REG1 != 0

            // Once REG1 reaches 0, the loop ends, and the next instruction executes
            file << "LOAD REG3 10\n";
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

    // Step 1: Load instructions and check state after each one

    // Execute first instruction: LOAD REG0 10
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 10); // REG0 should be 10

    // Execute second instruction: LOAD REG1 20
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 20); // REG1 should be 20

    // Execute third instruction: INC REG0
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 11); // REG0 should be 11

    // Execute fourth instruction: DEC REG1
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 19); // REG1 should be 19

    // Execute fifth instruction: JNZ [end] (REG1 != 0)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 6); // PC should jump to the instruction after INC REG1

    // Execute sixth instruction: INC REG1 (this should not execute)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 19); // REG1 should still be 19

    // Final check: CPU state should match expected final state

    std::cout << std::format("Test {}: CPU ExecuteNextInstruction function: PASSED\n", testNumber);
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

    // Execute first instruction: LOAD REG0 10
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 10); // REG0 should be 10

    // Execute second instruction: LOAD REG1 1
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 1); // REG1 should be 1

    // Execute third instruction: LOAD REG2 0
    cpu.executeNextInstruction();
    assert(cpu.getRegister(2) == 0); // REG2 should be 0

    // Execute fourth instruction: INC REG0
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 11); // REG0 should be 11

    // Execute fifth instruction: DEC REG1 (REG1 becomes 0)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 0); // REG1 should be 0

    // Execute sixth instruction: JNZ [label1] (REG1 == 0, so it should NOT jump)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 6); // PC should NOT jump, continue to the next instruction (which is INC REG2)

    // Execute seventh instruction: INC REG2 (This should be executed after JNZ does NOT jump)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(2) == 1); // REG2 should be 1

    // Execute eighth instruction: INC REG2 (This should be executed after JNZ does NOT jump)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(2) == 2); // REG2 should be 1

    // Execute ninth instruction: INC REG1 (Reset REG1)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 1); // REG1 should be 1

    // Execute tenth instruction: DEC REG0
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 10); // REG0 should be 10

    // Execute eleventh instruction: JNZ [label2] (REG0 != 0, so it should jump)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 12); // PC should jump to label2

    // Execute twelfth instruction: INC REG2 (In label2)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(2) == 3); // REG2 should be 2

    // Execute thirteenth instruction: INC REG0 (This happens after jump)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(0) == 11); // REG0 should be 11

    // Final check: CPU state should match expected final state
    std::cout << std::format("Test {}: CPU ExecuteNextInstruction Case 5: PASSED\n", testNumber);
    std::cout << "------------" << std::endl;
    testNumber++;
}

void testCPUWithFileLoop(int cpu_id, const std::string &mockFileName)
{
    totalTests++;
    Compiler compiler;

    std::vector<std::string> instructions = compiler.loadInstructionsFromFile(mockFileName);

    CPU cpu(cpu_id, instructions);

    // Step 1: Execute instructions and check register values after each one

    // Execute first instruction: LOAD REG3 0
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 0); // REG3 should be 0

    // Execute second instruction: LOAD REG1 10
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 10); // REG1 should be 10

    // Execute third instruction: loop_start:
    cpu.executeNextInstruction();
    // No assertion here, just a label

    // Execute fourth instruction: INC REG3 (REG3 becomes 1)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 1); // REG3 should be 1 after 1st increment

    // Execute fifth instruction: DEC REG1 (REG1 becomes 9)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 9); // REG1 should be 9 after 1st decrement

    // Execute sixth instruction: JNZ [loop_start] (REG1 != 0, so jump back)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 3); // PC should jump back to loop_start (position 3)

    // Execute fourth instruction again (second iteration): INC REG3 (REG3 becomes 2)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 2); // REG3 should be 2 after 2nd increment

    // Execute fifth instruction again (second iteration): DEC REG1 (REG1 becomes 8)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 8); // REG1 should be 8 after 2nd decrement

    // Execute sixth instruction again: JNZ [loop_start] (REG1 != 0, so jump back)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 3); // PC should jump back to loop_start (position 3)

    // Execute fourth instruction again (third iteration): INC REG3 (REG3 becomes 3)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 3); // REG3 should be 3 after 3rd increment

    // Execute fifth instruction again (third iteration): DEC REG1 (REG1 becomes 7)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 7); // REG1 should be 7 after 3rd decrement

    // Execute sixth instruction again: JNZ [loop_start] (REG1 != 0, so jump back)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 3); // PC should jump back to loop_start (position 3)

    // Execute fourth instruction again (fourth iteration): INC REG3 (REG3 becomes 4)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 4); // REG3 should be 4 after 4th increment

    // Execute fifth instruction again (fourth iteration): DEC REG1 (REG1 becomes 6)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 6); // REG1 should be 6 after 4th decrement

    // Execute sixth instruction again: JNZ [loop_start] (REG1 != 0, so jump back)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 3); // PC should jump back to loop_start (position 3)

    // Execute fourth instruction again (fifth iteration): INC REG3 (REG3 becomes 5)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 5); // REG3 should be 5 after 5th increment

    // Execute fifth instruction again (fifth iteration): DEC REG1 (REG1 becomes 5)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 5); // REG1 should be 5 after 5th decrement

    // Execute sixth instruction again: JNZ [loop_start] (REG1 != 0, so jump back)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 3); // PC should jump back to loop_start (position 3)

    // Execute fourth instruction again (sixth iteration): INC REG3 (REG3 becomes 6)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 6); // REG3 should be 6 after 6th increment

    // Execute fifth instruction again (sixth iteration): DEC REG1 (REG1 becomes 4)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 4); // REG1 should be 4 after 6th decrement

    // Execute sixth instruction again: JNZ [loop_start] (REG1 != 0, so jump back)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 3); // PC should jump back to loop_start (position 3)

    // Execute fourth instruction again (seventh iteration): INC REG3 (REG3 becomes 7)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 7); // REG3 should be 7 after 7th increment

    // Execute fifth instruction again (seventh iteration): DEC REG1 (REG1 becomes 3)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 3); // REG1 should be 3 after 7th decrement

    // Execute sixth instruction again: JNZ [loop_start] (REG1 != 0, so jump back)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 3); // PC should jump back to loop_start (position 3)

    // Execute fourth instruction again (eighth iteration): INC REG3 (REG3 becomes 8)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 8); // REG3 should be 8 after 8th increment

    // Execute fifth instruction again (eighth iteration): DEC REG1 (REG1 becomes 2)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 2); // REG1 should be 2 after 8th decrement

    // Execute sixth instruction again: JNZ [loop_start] (REG1 != 0, so jump back)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 3); // PC should jump back to loop_start (position 3)

    // Execute fourth instruction again (ninth iteration): INC REG3 (REG3 becomes 9)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 9); // REG3 should be 9 after 9th increment

    // Execute fifth instruction again (ninth iteration): DEC REG1 (REG1 becomes 1)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 1); // REG1 should be 1 after 9th decrement

    // Execute sixth instruction again: JNZ [loop_start] (REG1 != 0, so jump back)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 3); // PC should jump back to loop_start (position 3)

    // Execute fourth instruction again (tenth iteration): INC REG3 (REG3 becomes 10)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 10); // REG3 should be 10 after 10th increment

    // Execute fifth instruction again (tenth iteration): DEC REG1 (REG1 becomes 0)
    cpu.executeNextInstruction();
    assert(cpu.getRegister(1) == 0); // REG1 should be 0 after 10th decrement

    // Execute sixth instruction again: JNZ [loop_start] (REG1 == 0, so do NOT jump)
    cpu.executeNextInstruction();
    assert(cpu.getPC() == 6); // PC should move to the next instruction (position 13)

    // Execute final instruction: LOAD REG3 10
    cpu.executeNextInstruction();
    assert(cpu.getRegister(3) == 10); // REG3 should be 10

    // Final check: CPU state should match expected final state
    std::cout << std::format("Test {}: CPU Loop Case 6: PASSED\n", testNumber);
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
