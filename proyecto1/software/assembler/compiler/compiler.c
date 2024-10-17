// compiler.c

#include "compiler.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** dummy_ptr;
char* tagNames[100];
int tagPositions[100];
int tagCounter = 0;
int lineCounter = -1;

typedef struct {
  char* func_name;
  unsigned int op;
  unsigned int func3;
  unsigned int func11;
} instructions;
const instructions inst[] = {
    {.func_name = "SUM", .func3 = 0b000, .op = 0b000, .func11 = 0b00000000000},
    {.func_name = "DIF", .func3 = 0b001, .op = 0b000, .func11 = 0b00000000000},
    {.func_name = "AND", .func3 = 0b010, .op = 0b000, .func11 = 0b00000000000},
    {.func_name = "OR", .func3 = 0b011, .op = 0b000, .func11 = 0b00000000000},
    {.func_name = "XOR", .func3 = 0b100, .op = 0b000, .func11 = 0b00000000000},
    {.func_name = "SLL", .func3 = 0b101, .op = 0b000, .func11 = 0b00000000000},
    {.func_name = "SLR", .func3 = 0b110, .op = 0b000, .func11 = 0b00000000000},
    {.func_name = "SAR", .func3 = 0b111, .op = 0b000, .func11 = 0b00000000000},
    {.func_name = "MUL", .func3 = 0b000, .op = 0b000, .func11 = 0b00000000001},
    {.func_name = "DIV", .func3 = 0b100, .op = 0b000, .func11 = 0b00000000001},
    {.func_name = "SUMI", .func3 = 0b000, .op = 0b001, .func11 = 0b00000000000},
    {.func_name = "DIFI", .func3 = 0b001, .op = 0b001, .func11 = 0b00000000000},
    {.func_name = "ANDI", .func3 = 0b010, .op = 0b001, .func11 = 0b00000000000},
    {.func_name = "ORI", .func3 = 0b011, .op = 0b001, .func11 = 0b00000000000},
    {.func_name = "XORI", .func3 = 0b100, .op = 0b001, .func11 = 0b00000000000},
    {.func_name = "SLLI", .func3 = 0b101, .op = 0b001, .func11 = 0b00000000000},
    {.func_name = "SLRI", .func3 = 0b110, .op = 0b001, .func11 = 0b00000000000},
    {.func_name = "SARI", .func3 = 0b111, .op = 0b001, .func11 = 0b00000000000},
    {.func_name = "STM", .func3 = 0b000, .op = 0b010, .func11 = 0b00000000000},
    {.func_name = "CLIR", .func3 = 0b000, .op = 0b011, .func11 = 0b00000000000},
    {.func_name = "CUIR", .func3 = 0b001, .op = 0b011, .func11 = 0b00000000000},
    {.func_name = "JLL", .func3 = 0b010, .op = 0b011, .func11 = 0b00000000000},
    {.func_name = "LDM", .func3 = 0b000, .op = 0b101, .func11 = 0b00000000000},
    {.func_name = "JLRL", .func3 = 0b010, .op = 0b101, .func11 = 0b00000000000},
    {.func_name = "JIEQ", .func3 = 0b000, .op = 0b110, .func11 = 0b00000000000},
    {.func_name = "JINE", .func3 = 0b001, .op = 0b110, .func11 = 0b00000000000},
    {.func_name = "JIGT", .func3 = 0b010, .op = 0b110, .func11 = 0b00000000000},
    {.func_name = "JILT", .func3 = 0b011, .op = 0b110, .func11 = 0b00000000000},
    {.func_name = "JIGE", .func3 = 0b100, .op = 0b110, .func11 = 0b00000000000},
    {.func_name = "JILE", .func3 = 0b101, .op = 0b110, .func11 = 0b00000000000},
    // vectoriales
    {.func_name = "LDV", .func3 = 0b100, .op = 0b101, .func11 = 0b00000000000},
    {.func_name = "STV", .func3 = 0b100, .op = 0b010, .func11 = 0b00000000000},
    // tipo E
    {.func_name = "VXOR",       .func3 = 0b000, .op = 0b100, .func11 = 0b00000000000},
    {.func_name = "VROT32",     .func3 = 0b001, .op = 0b100, .func11 = 0b00000000000},
    {.func_name = "VAES_SUB",   .func3 = 0b000, .op = 0b100, .func11 = 0b00000000001},
    {.func_name = "VAES_ISUB",  .func3 = 0b001, .op = 0b100, .func11 = 0b00000000001},
    {.func_name = "VAES_SHIFT", .func3 = 0b010, .op = 0b100, .func11 = 0b00000000001},
    {.func_name = "VAES_MIX",   .func3 = 0b011, .op = 0b100, .func11 = 0b00000000001},
    {.func_name = "VAES_WXOR",  .func3 = 0b100, .op = 0b100, .func11 = 0b00000000001},
};
typedef struct {
  char* name;
  unsigned int val;
} reg_spec;
const reg_spec regs[] = {{.name = "x0", .val = 0},
                         {.name = "x1", .val = 1},
                         {.name = "x2", .val = 2},
                         {.name = "x3", .val = 3},
                         {.name = "x4", .val = 4},
                         {.name = "x5", .val = 5},
                         {.name = "x6", .val = 6},
                         {.name = "x7", .val = 7},
                         {.name = "x8", .val = 8},
                         {.name = "x9", .val = 9},
                         {.name = "x10", .val = 10},
                         {.name = "x11", .val = 11},
                         {.name = "x12", .val = 12},
                         {.name = "x13", .val = 13},
                         {.name = "x14", .val = 14},
                         {.name = "x15", .val = 15},
                         {.name = "x16", .val = 16},
                         {.name = "x17", .val = 17},
                         {.name = "x18", .val = 18},
                         {.name = "x19", .val = 19},
                         {.name = "x20", .val = 20},
                         {.name = "x21", .val = 21},
                         {.name = "x22", .val = 22},
                         {.name = "x23", .val = 23},
                         {.name = "x24", .val = 24},
                         {.name = "x25", .val = 25},
                         {.name = "x26", .val = 26},
                         {.name = "x27", .val = 27},
                         {.name = "x28", .val = 28},
                         {.name = "x29", .val = 29},
                         {.name = "x30", .val = 30},
                         {.name = "x31", .val = 31},
                         // aliases
                         {.name = "zero", .val = 0},
                         {.name = "lr", .val = 1},
                         {.name = "sp", .val = 2},
                         {.name = "g0", .val = 3},
                         {.name = "g1", .val = 4},
                         {.name = "g2", .val = 5},
                         {.name = "g3", .val = 6},
                         {.name = "g4", .val = 7},
                         {.name = "g5", .val = 8},
                         {.name = "g6", .val = 9},
                         {.name = "g7", .val = 10},
                         {.name = "t0", .val = 11},
                         {.name = "t1", .val = 12},
                         {.name = "t2", .val = 13},
                         {.name = "t3", .val = 14},
                         {.name = "t4", .val = 15},
                         {.name = "t5", .val = 16},
                         {.name = "t6", .val = 17},
                         {.name = "t7", .val = 18},
                         {.name = "arg0", .val = 19},
                         {.name = "arg1", .val = 20},
                         {.name = "arg2", .val = 21},
                         {.name = "arg3", .val = 22},
                         {.name = "arg4", .val = 23},
                         {.name = "v0", .val = 24},
                         {.name = "v1", .val = 25},
                         {.name = "v2", .val = 26},
                         {.name = "v3", .val = 27},
                         {.name = "v4", .val = 28},
                         {.name = "v5", .val = 29},
                         {.name = "v6", .val = 30},
                         {.name = "v7", .val = 31}};

// funciones

void reverse_string(char* str) {
  int length = strlen(str);
  int i, j;

  for (i = 0, j = length - 1; i < j; i++, j--) {
    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;
  }
}
unsigned int string2reg(char* regName) {
  for (size_t i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
    if (strcmp(regName, regs[i].name) == 0) {
      return regs[i].val;
    }
  }
  fprintf(stderr, "Invalid register value '%s' at line %d\n", regName,
          lineCounter);
  exit(1);
  return (unsigned int)-1;
}

unsigned int string2funct3(char* funcName) {
  for (size_t i = 0; i < sizeof(inst) / sizeof(inst[0]); i++) {
    if (strcmp(funcName, inst[i].func_name) == 0) {
      return inst[i].func3;
    }
  }
  fprintf(stderr, "Invalid function name '%s' at line %d\n", funcName,
          lineCounter);
  exit(1);
  return (unsigned int)-1;
}

unsigned int string2funct11(char* funcName) {
  for (size_t i = 0; i < sizeof(inst) / sizeof(inst[0]); i++) {
    if (strcmp(funcName, inst[i].func_name) == 0) {
      return inst[i].func11;
    }
  }
  fprintf(stderr, "Invalid function name '%s' at line %d\n", funcName,
          lineCounter);
  exit(1);
  return (unsigned int)-1;
}

unsigned int string2op(char* funcName) {
  for (size_t i = 0; i < sizeof(inst) / sizeof(inst[0]); i++) {
    if (strcmp(funcName, inst[i].func_name) == 0) {
      return inst[i].op;
    }
  }
  fprintf(stderr, "Invalid op name '%s' at line %d\n", funcName, lineCounter);
  exit(1);
  return (unsigned int)-1;
}

char* int2bin(int num, int numBits) {
  if (numBits <= 0) {
    fprintf(stderr, "Invalid number of bits.\n");
    return NULL;
  }

  char* binaryString = (char*)malloc(numBits + 1);  // +1 for null terminator

  if (binaryString == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);  // Handle memory allocation failure
  }

  // Determine the most significant bit
  int signBit = (num < 0) ? 1 : 0;

  // If it's negative, take two's complement
  if (num < 0) {
    num = (1 << numBits) + num;
  }

  for (int i = 0; i < numBits; i++) {
    int bit = (num >> (numBits - 1 - i)) & 1;
    binaryString[i] = bit ? '1' : '0';
  }

  binaryString[numBits] = '\0';  // Null-terminate the string

  // Apply the sign bit
  binaryString[0] = (signBit == 1) ? '1' : binaryString[0];

  return binaryString;
}

char* handle_instruction(char* parts[], int token_counter) {
  char* binaryString;

  int operation = string2op(parts[0]);

  // printf("op %d ,",operation);
  // printf("opstr %s ",parts[0]);
  // elegir traduccion segun el tipo de instruccion

  lineCounter++;
  if (operation == 0b000) {
    binaryString = typeA_assembly2bin(parts[0], parts[1], parts[2], parts[3]);
    // printf("Type A \n");
  } else if (operation == 0b001) {
    binaryString = typeB_assembly2bin(parts[0], parts[1], parts[2], parts[3]);
    // printf("Type B \n");
  } else if (operation == 0b010) {
    // registros en orden invertido
    binaryString = typeC_assembly2bin(parts[0], parts[2], parts[1], parts[3]);
    // printf("Type C \n");
  } else if (operation == 0b011) {
    binaryString = typeD_assembly2bin(parts[0], parts[1], parts[2]);
    // printf("Type D \n");
  } else if (operation == 0b100) {
    binaryString = typeE_assembly2bin(parts[0], parts[1], parts[2], parts[3]);
    // printf("Type E \n");
  } else if (operation == 0b101) {
    binaryString = typeF_assembly2bin(parts[0], parts[1], parts[2], parts[3]);
    // printf("Type F \n");
  } else if (operation == 0b110) {
    binaryString = typeG_assembly2bin(parts[0], parts[1], parts[2], parts[3]);
    // printf("Type G \n");
  } else {
    fprintf(stderr, "Invalid instruction '%s' at line %d\n", parts[0],
            lineCounter);
    exit(1);
  }
  // printf("%d %s\n", lineCounter,binaryString);
  printf("%s\n", binaryString);

  // printf("lines %d \n", lines);
}

char* typeA_assembly2bin(char* assembly_instruction, char* rd, char* reg1,
                         char* reg2) {
  // Para SUM, DIF, AND, OR, XOR, SLL, SLR, SAR, MUL, DIV
  const char* opcode_str = "000";
  const char* func3_str = int2bin(string2funct3(assembly_instruction), 3);
  const char* reg_str = int2bin(string2reg(rd), 5);
  const char* reg1_str = int2bin(string2reg(reg1), 5);
  const char* reg2_str = int2bin(string2reg(reg2), 5);
  const char* func11_str = int2bin(string2funct11(assembly_instruction), 11);

  size_t totalLength = strlen(opcode_str) + strlen(func3_str) +
                       strlen(reg_str) + strlen(reg2_str) + strlen(reg1_str) +
                       strlen(func11_str) + 1;  // para terminar en null

  char* result = (char*)malloc(totalLength);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }

  result[0] = '\0';

  strcat(result, func11_str);
  strcat(result, reg2_str);
  strcat(result, reg1_str);
  strcat(result, reg_str);
  strcat(result, func3_str);
  strcat(result, opcode_str);

  return result;
}

char* typeB_assembly2bin(char* assembly_instruction, char* rd, char* reg1,
                         char* inmm) {
  // Para SUMI, DIFI, ANDI, ORI, XORI, SLLI, SLRI, SARI

  const char* opcode_str = "001";
  const char* func3_str = int2bin(string2funct3(assembly_instruction), 3);
  const char* reg_str = int2bin(string2reg(rd), 5);
  const char* reg1_str = int2bin(string2reg(reg1), 5);
  // int inmm_int = atoi(inmm);
  int inmm_int = (int)strtol(inmm, dummy_ptr, 0);
  if ((inmm_int == 0) && strcmp(inmm, "0") && strcmp(inmm, "0x0")) {
    fprintf(stderr, "Unrecognized symbol as inmediate '%s' line %d \n", inmm,
            lineCounter);
    exit(1);
  }
  const char* inmm_str = int2bin(inmm_int, 16);

  size_t totalLength = strlen(opcode_str) + strlen(func3_str) +
                       strlen(reg_str) + strlen(inmm_str) + strlen(reg1_str) +
                       1;  // para terminar en null

  char* result = (char*)malloc(totalLength);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }

  result[0] = '\0';
  strcat(result, inmm_str);
  strcat(result, reg1_str);
  strcat(result, reg_str);
  strcat(result, func3_str);
  strcat(result, opcode_str);
  return result;
}

char* typeC_assembly2bin(char* assembly_instruction, char* reg1, char* reg2,
                         char* inmm) {
  // Para STM
  const char* opcode_str = "010";
  const char* func3_str = int2bin(string2funct3(assembly_instruction), 3);
  const char* reg1_str = int2bin(string2reg(reg1), 5);
  const char* reg2_str = int2bin(string2reg(reg2), 5);
  // int inmm_int = atoi(inmm);
  int inmm_int = (int)strtol(inmm, dummy_ptr, 0);
  if ((inmm_int == 0) && strcmp(inmm, "0") && strcmp(inmm, "0x0")) {
    fprintf(stderr, "Unrecognized symbol as inmediate '%s' line %d \n", inmm,
            lineCounter);
    exit(1);
  }
  char* inmm_str = int2bin(inmm_int, 16);

  size_t totalLength = strlen(opcode_str) + strlen(func3_str) +
                       strlen(reg2_str) + strlen(inmm_str) + strlen(reg1_str) +
                       1;  // para terminar en null

  char* result = (char*)malloc(totalLength);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }

  result[0] = '\0';

  // reverse_string(inmm_str);
  // 01000000000000001010000100011111
  strncat(result, inmm_str, 11);
  strcat(result, reg2_str);
  strcat(result, reg1_str);
  strncat(result, inmm_str + 11, 5);
  strcat(result, func3_str);
  strcat(result, opcode_str);
  return result;
}

char* typeD_assembly2bin(char* assembly_instruction, char* rd, char* inmm) {
  // Para CLIR, CUIR, JLL
  const char* opcode_str = "011";
  const char* func3_str = int2bin(string2funct3(assembly_instruction), 3);
  const char* reg_str = int2bin(string2reg(rd), 5);
  const char* inmm_str;

  if (strcmp(func3_str, "010")) {
    // const int inmm_int = atoi(inmm);
    int inmm_int = (int)strtol(inmm, dummy_ptr, 0);
    if ((inmm_int == 0) && strcmp(inmm, "0") && strcmp(inmm, "0x0")) {
      fprintf(stderr, "Unrecognized symbol as inmediate '%s' line %d \n", inmm,
              lineCounter);
      exit(1);
    }
    inmm_str = int2bin(inmm_int, 21);
  } else {
    // inmm es un label
    int label_pos = find_position_by_tag(inmm);
    inmm_str = int2bin(4 * (label_pos - lineCounter), 21);
    // printf("\nlabel_pos %d, inmm %s, lineCounter %d, inmm_srt %s\n",
    // label_pos, inmm, lineCounter, inmm_str);
  }
  size_t totalLength = strlen(opcode_str) + strlen(func3_str) +
                       strlen(reg_str) + strlen(inmm_str) +
                       1;  // para terminar en null

  char* result = (char*)malloc(totalLength);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }

  result[0] = '\0';

  strcat(result, inmm_str);
  strcat(result, reg_str);
  strcat(result, func3_str);
  strcat(result, opcode_str);

  return result;
}

// vectorial
char* typeE_assembly2bin(char* assembly_instruction, char* rd, char* reg1,
                         char* reg2) {
  // Para SUM, DIF, AND, OR, XOR, SLL, SLR, SAR, MUL, DIV
  const char* opcode_str = "100";
  const char* func3_str = int2bin(string2funct3(assembly_instruction), 3);
  const char* reg_str = int2bin(string2reg(rd), 5);
  const char* reg1_str = int2bin(string2reg(reg1), 5);
  const char* reg2_str = int2bin(string2reg(reg2), 5);
  const char* func11_str = int2bin(string2funct11(assembly_instruction), 11);

  size_t totalLength = strlen(opcode_str) + strlen(func3_str) +
                       strlen(reg_str) + strlen(reg2_str) + strlen(reg1_str) +
                       strlen(func11_str) + 1;  // para terminar en null

  char* result = (char*)malloc(totalLength);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }

  result[0] = '\0';

  strcat(result, func11_str);
  strcat(result, reg2_str);
  strcat(result, reg1_str);
  strcat(result, reg_str);
  strcat(result, func3_str);
  strcat(result, opcode_str);

  return result;
}

char* typeF_assembly2bin(char* assembly_instruction, char* reg1, char* reg2,
                         char* inmm) {
  const char* opcode_str = "101";
  const char* func3_str = int2bin(string2funct3(assembly_instruction), 3);
  const char* reg_str = int2bin(string2reg(reg1), 5);
  const char* reg1_str = int2bin(string2reg(reg2), 5);
  const char* inmm_str;
  // const int inmm_int = atoi(inmm);
  int inmm_int = (int)strtol(inmm, dummy_ptr, 0);
  if ((inmm_int == 0) && strcmp(inmm, "0") && strcmp(inmm, "0x0")) {
    fprintf(stderr, "Unrecognized symbol as inmediate '%s' line %d \n", inmm,
            lineCounter);
    exit(1);
  }
  inmm_str = int2bin(inmm_int, 16);
  size_t totalLength = strlen(opcode_str) + strlen(func3_str) +
                       strlen(reg_str) + strlen(inmm_str) + strlen(reg1_str) +
                       1;  // para terminar en null

  char* result = (char*)malloc(totalLength);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }

  result[0] = '\0';

  strcat(result, inmm_str);
  strcat(result, reg1_str);
  strcat(result, reg_str);
  strcat(result, func3_str);
  strcat(result, opcode_str);
  return result;
}

char* typeG_assembly2bin(char* assembly_instruction, char* reg1, char* reg2,
                         char* inmm) {
  // Para JIEQ, JINE, JIGT, JILT, JIGE, JILE
  const char* opcode_str = "110";
  const char* func3_str = int2bin(string2funct3(assembly_instruction), 3);
  const char* reg1_str = int2bin(string2reg(reg1), 5);
  const char* reg2_str = int2bin(string2reg(reg2), 5);
  int label_pos = find_position_by_tag(inmm);
  int inmm_calc = 4 * (label_pos - lineCounter);
  char* inmm_str = int2bin(inmm_calc, 18);

  size_t totalLength = strlen(opcode_str) + strlen(func3_str) +
                       strlen(reg2_str) + strlen(inmm_str) + strlen(reg1_str) +
                       1;  // para terminar en null

  char* result = (char*)malloc(totalLength);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }

  // reverse_string(inmm_str);

  result[0] = '\0';
  strncat(result, inmm_str, 11);
  strcat(result, reg2_str);
  strcat(result, reg1_str);
  strncat(result, inmm_str + 11, 5);
  strcat(result, func3_str);
  strcat(result, opcode_str);
  return result;
}

int save_label_address(char* tag, int line) {
  // printf("\ntagcounter %d tag %s \n",tagCounter, tag);
  tagNames[tagCounter] =
      (char*)malloc(strlen(tag) + 1);  // +1 for the null terminator
  if (tagNames[tagCounter] == NULL) {
    exit(1);
  }
  strcpy(tagNames[tagCounter], tag);
  tagPositions[tagCounter] = line;
  tagCounter++;
  // for (int i = 0; i < 5; i++) { // Adjust the loop range based on the number
  // of elements
  //     printf("Tag Name: %s, Position: %d\n", tagNames[i], tagPositions[i]);
  // }
  return tagCounter;
}

int find_position_by_tag(char* tag) {
  for (int i = 0; i < tagCounter; i++) {
    if (strcmp(tag, tagNames[i]) == 0) {
      return tagPositions[i];
    }
  }
  fprintf(stderr, "Label not recognized: '%s' at line %d\n", tag, lineCounter);
  exit(1);
  // Return a sentinel value to indicate that the tag was not found
  return -1;
}

bool is_line(const char* str) {
  if (str == NULL || str[0] == '\0') {
    return false;  //
  }

  size_t len = strlen(str);
  return str[len - 2] == ';';
}
