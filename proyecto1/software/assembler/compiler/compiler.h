// compiler.h

#include <stdbool.h>
#ifndef COMPILER_H
#define COMPILER_H


/**
 * The function `int2bin` converts an integer to its binary representation with a specified number of
 * bits.
 * 
 * @param num The `num` parameter is the integer number that you want to convert to binary.
 * @param numBits The `numBits` parameter is the number of bits to represent the integer `num` in
 * binary form.
 * 
 * @return The function `int2bin` returns a pointer to a character array (string) representing the
 * binary representation of the input number.
 */
char* int2bin(int num, int numBits);
/**
 * The function `reverse_string` takes a string as input and reverses its characters in place.
 * 
 * @param str The parameter `str` is a pointer to a character array, which represents the string that
 * we want to reverse.
 */
void reverse_string(char* str);
/**
 * The function `typeA_assembly2bin` converts an assembly instruction into its binary representation.
 * 
 * @param assembly_instruction The assembly instruction is a string that represents the type of
 * operation to be performed. It could be "SUM", "DIF", "AND", "OR", "XOR", "SLL", "SLR", or "SAR".
 * @param rd The parameter "rd" in the function "typeA_assembly2bin" represents the destination
 * register. It is the register where the result of the operation will be stored.
 * @param reg1 The parameter "reg1" in the function `typeA_assembly2bin` represents the first register
 * operand in the assembly instruction. It is a string that specifies the register name or number.
 * @param reg2 The parameter "reg2" in the function "typeA_assembly2bin" represents the second register
 * operand in the assembly instruction. It is used to specify the register that will be used as an
 * input for the operation.
 * 
 * @return The function `typeA_assembly2bin` returns a dynamically allocated character array (string)
 * that represents the binary encoding of the given assembly instruction.
 */
char *typeA_assembly2bin(char *assembly_instruction, char *reg, char *reg_or_inmm1, char *reg_or_inmm2);
/**
 * The function `typeB_assembly2bin` converts assembly instructions of type B (SUMI, DIFI, ANDI, ORI,
 * XORI, SLLI, SLRI, SARI) into binary format.
 * 
 * @param assembly_instruction The `assembly_instruction` parameter is a string that represents the
 * assembly instruction being converted to binary. It could be one of the following instructions: SUMI,
 * DIFI, ANDI, ORI, XORI, SLLI, SLRI, or SARI.
 * @param rd The "rd" parameter in the function `typeB_assembly2bin` represents the destination
 * register. It is a character array that contains the name or number of the register where the result
 * of the instruction will be stored.
 * @param reg1 The parameter "reg1" in the function "typeB_assembly2bin" represents the name of a
 * register. It is used to specify the source register for certain instructions like SUMI, DIFI, ANDI,
 * ORI, XORI, SLLI, SLRI, and S
 * @param inmm The parameter "inmm" is a string representing an immediate value that will be used in
 * the assembly instruction.
 * 
 * @return The function `typeB_assembly2bin` returns a dynamically allocated character array (string)
 * that represents the binary encoding of the given assembly instruction.
 */
char *typeB_assembly2bin(char *assembly_instruction, char *rd, char *reg1, char *inmm);
/**
 * The function `typeC_assembly2bin` converts a C assembly instruction into its binary representation.
 * 
 * @param assembly_instruction A string representing the assembly instruction (e.g., "SUMI", "DIFI",
 * "ANDI", etc.).
 * @param reg1 The `reg1` parameter is a string representing the first register in the assembly
 * instruction.
 * @param reg2 The `reg2` parameter in the `typeC_assembly2bin` function is a string representing the
 * second register operand in the assembly instruction. It is used to convert the register name into
 * its binary representation.
 * @param inmm The parameter "inmm" is a string representing an immediate value. It is converted to an
 * integer using the atoi() function and then converted to a binary string using the int2bin()
 * function.
 * 
 * @return The function `typeC_assembly2bin` returns a dynamically allocated character array (string)
 * that represents the binary encoding of the given assembly instruction, registers, and immediate
 * value.
 */
char *typeC_assembly2bin(char *assembly_instruction, char *inmm, char *reg1, char *reg2);
/**
 * The function `typeD_assembly2bin` converts assembly instructions of type D into binary format.
 * 
 * @param assembly_instruction The assembly instruction is a string that represents the type D
 * instruction. It can be one of the following instructions: CLIR, CUIR, or JLL.
 * @param rd The parameter "rd" in the function `typeD_assembly2bin` represents the destination
 * register in the assembly instruction. It is a character array that contains the register name or
 * number.
 * @param inmm The parameter "inmm" in the function "typeD_assembly2bin" is a character array that
 * represents the immediate value or label used in the assembly instruction.
 * 
 * @return The function `typeD_assembly2bin` returns a pointer to a character array (string) that
 * represents the binary encoding of the given assembly instruction, rd (register destination), and
 * inmm (immediate value or label).
 */
char *typeD_assembly2bin(char *assembly_instruction, char *rd, char *inmm);

/**
 * The function `typeE_assembly2bin` converts an assembly instruction into its binary representation.
 * 
 * @param assembly_instruction The assembly instruction is a string that represents the type of
 * operation to be performed. 
 * @param rd The parameter "rd" in the function "typeE_assembly2bin" represents the destination
 * register. It is the register where the result of the operation will be stored.
 * @param reg1 The parameter "reg1" in the function `typeE_assembly2bin` represents the first register
 * operand in the assembly instruction. It is a string that specifies the register name or number.
 * @param reg2 The parameter "reg2" in the function "typeE_assembly2bin" represents the second register
 * operand in the assembly instruction. It is used to specify the register that will be used as an
 * input for the operation.
 * 
 * @return The function `typeE_assembly2bin` returns a dynamically allocated character array (string)
 * that represents the binary encoding of the given assembly instruction.
 */
char *typeE_assembly2bin(char *assembly_instruction, char *reg, char *reg_or_inmm1, char *reg_or_inmm2);
/**
 * The function `typeF_assembly2bin` converts an assembly instruction into its binary representation
 * for a specific instruction format.
 * 
 * @param assembly_instruction The `assembly_instruction` parameter is a string that represents the
 * assembly instruction. It is used to determine the `func3` value for the binary representation.
 * @param reg1 The parameter "reg1" in the function `typeF_assembly2bin` represents the first register
 * operand in the assembly instruction. It is a character array (string) that contains the name or
 * identifier of the register.
 * @param reg2 The parameter "reg2" is a character array that represents the second register in the
 * assembly instruction.
 * @param inmm The parameter "inmm" in the function "typeF_assembly2bin" is used to represent an
 * immediate value or a label. If the value of "func3_str" is not equal to "010", then "inmm" is
 * treated as an immediate value and converted to binary using
 * 
 * @return The function `typeF_assembly2bin` returns a pointer to a character array (`char*`).
 */
char *typeF_assembly2bin(char *assembly_instruction, char *reg1, char *reg2, char *inmm);

/**
 * The function `typeG_assembly2bin` converts an assembly instruction, along with register and
 * immediate values, into a binary representation.
 * 
 * @param assembly_instruction A string representing the assembly instruction. This could be something
 * like "add", "sub", "mul", etc.
 * @param reg1 The `reg1` parameter is a string representing the first register in the assembly
 * instruction.
 * @param reg2 The parameter "reg2" is a string representing a register in assembly language. It is
 * used in the function `typeG_assembly2bin` to convert the assembly instruction into binary code.
 * @param inmm The parameter "inmm" is a string that represents an immediate value or a memory address.
 * It is used in the assembly instruction to specify a specific memory location or a constant value.
 * 
 * @return a pointer to a character array, which represents the binary representation of the given
 * assembly instruction, registers, and immediate value.
 */
char *typeG_assembly2bin(char *assembly_instruction, char *reg1, char *reg2, char *inmm);
/**
 * The function "find_position_by_tag" searches for a given tag in an array and returns its
 * corresponding position.
 * 
 * @param tag The "tag" parameter is a pointer to a character array (string) that represents the tag we
 * are searching for.
 * 
 * @return The function `find_position_by_tag` returns the position of a tag in an array. If the tag is
 * found, the function returns the corresponding position. If the tag is not found, the function
 * returns -1 as a sentinel value to indicate that the tag was not found.
 */

int find_position_by_tag(char* tag);
/**
 * The function checks if a given string is a line of code.
 * 
 * @param str The parameter `str` is a pointer to a character array, which represents a string.
 */
bool is_line(const char *str);
/**
 * The function `save_label_address` saves the address of a label along with its corresponding line
 * number.
 * 
 * @param tag A string representing the tag name to be saved.
 * @param line The line parameter is an integer that represents the line number where the tag is
 * located.
 * 
 * @return The function `save_label_address` returns the value of the variable `tagCounter`.
 */
int save_label_address(char *tag, int line);
/**
 * The function `handle_instruction` takes in an array of instruction parts and a token counter,
 * determines the type of instruction, and converts it into a binary string.
 * 
 * @param parts An array of strings representing the different parts of the instruction. The size of
 * the array is given by the parameter token_counter.
 * @param token_counter The parameter `token_counter` is an integer that represents the number of
 * tokens in the `parts` array. It is used to determine the type of instruction and to ensure that the
 * correct number of arguments are provided for each instruction type.
 */
char *handle_instruction(char* parts[], int token_counter);
#endif
