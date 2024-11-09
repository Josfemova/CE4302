#ifndef Compiler_HPP
#define Compiler_HPP

#include <map>
#include <string>
#include <vector>

class Compiler
{
public:
    Compiler();

    // Cambiar el tipo de retorno a std::vector<std::string>
    std::vector<std::string> loadInstructionsFromFile(const std::string &filename);
    void createLabelsMap();

    std::vector<std::string> instructionMemory;
    std::map<std::string, int> labels; // Mapa para almacenar etiquetas y sus direcciones
    int pc;                            // Contador de programa
};

#endif // Compiler_HPP
