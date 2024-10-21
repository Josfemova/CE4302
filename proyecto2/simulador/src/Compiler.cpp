#include "Compiler.hpp"
#include <fstream>
#include <sstream>
#include <format>
#include <regex>
#include <iostream>

// Constructor
Compiler::Compiler() : pc(0) {}

// Cargar instrucciones desde un archivo
std::vector<std::string> Compiler::loadInstructionsFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << std::format("Error: No se pudo abrir el archivo {}\n", filename);
        return {};
    }

    // Clean inst mem and reset pc
    instructionMemory.clear();
    pc = 0;

    std::string line;

    // Regular expressions
    std::regex load_regex(R"(LOAD REG([0-3]) (\d+))");
    std::regex store_regex(R"(STORE REG([0-3]) (\d+))");
    std::regex inc_regex(R"(INC REG([0-3]))");
    std::regex dec_regex(R"(DEC REG([0-3]))");
    std::regex jnz_regex(R"(JNZ \[([a-zA-Z_][a-zA-Z0-9_]*)\])");
    std::regex label_regex(R"(([a-zA-Z_][a-zA-Z0-9_]*):)");

    bool validInstructions = true;

    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            std::smatch match;

            if (std::regex_match(line, match, load_regex))
            {
                // Format: LOAD REGX VALUE
                int reg = std::stoi(match[1].str());
                int value = std::stoi(match[2].str());
                instructionMemory.push_back(std::format("LOAD {} {}", reg, value));
            }
            else if (std::regex_match(line, match, store_regex))
            {
                // Format: STORE REGX VALUE
                int reg = std::stoi(match[1].str());
                int value = std::stoi(match[2].str());
                instructionMemory.push_back(std::format("STORE {} {}", reg, value));
            }
            else if (std::regex_match(line, match, inc_regex))
            {
                // Format: INC REGX
                int reg = std::stoi(match[1].str());
                instructionMemory.push_back(std::format("INC {}", reg));
            }
            else if (std::regex_match(line, match, inc_regex))
            {
                // Format: DEC REGX
                int reg = std::stoi(match[1].str());
                instructionMemory.push_back(std::format("DEC {}", reg));
            }
            else if (std::regex_match(line, match, jnz_regex))
            {
                // Format: JNZ [label]
                std::string label = match[1].str();
                if (labels.find(label) != labels.end())
                {
                    int jumpAddress = labels[label];
                    instructionMemory.push_back(std::format("JNZ {}", jumpAddress));
                }
                else
                {
                    std::cerr << std::format("Error: Etiqueta no encontrada: '{}'\n", label);
                    validInstructions = false;
                    break;
                }
            }
            else if (std::regex_match(line, match, label_regex))
            {
                // Format: label:
                std::string label = match[1].str();
                labels[label] = pc;
            }
            else
            {
                std::cerr << std::format("Error: Instrucción no válida: '{}'\n", line);
                validInstructions = false;
                break;
            }
            pc++;
        }
    }

    file.close();

    if (!validInstructions)
    {
        instructionMemory.clear();
        std::cerr << "Error: No se cargaron instrucciones debido a errores de formato.\n";
        return {};
    }

    return instructionMemory;
}
