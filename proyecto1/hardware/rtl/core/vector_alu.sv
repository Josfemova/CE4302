typedef enum bit [3:0] {
  VALU_OP_XOR = 4'b0000,  // xor, doubles as aes add roundkey 
  VALU_OP_ROT = 4'b0001,  // rotwords, for key schedule 
  VALU_OP_AES_SUBBYTES = 4'b1000,  // aes sub-bytes 
  VALU_OP_AES_INV_SUBBYTES = 4'b1001,  // aes sub-bytes 
  VALU_OP_AES_SHIFTROWS = 4'b1010, // aes shift rows
  VALU_OP_AES_MIX_COLUMNS = 4'b1011, // aes mix columns
  VALU_OP_AES_KEYSCHE_XOR = 4'b1100 // el xor ese raro del key schedule con el resultado de la transformacion
} valu_op;

/// Como sacar el key schedule? 
/*
  Se agarra el key, se aplica rot, se aplica subbytes, 
  Se carga el Rcon a un reg scalar y se aplica el xor vectorial 
  Se aplica la instruccion del key schedule 
*/

module vector_alu #(
    parameter WIDTH = 128
) (
    input [WIDTH-1:0] op1,
    input [WIDTH-1:0] op2,
    input bit [3:0] alu_control,
    output logic [WIDTH-1:0] result
);
  always @(*) begin
    case (alu_control)
      VALU_OP_XOR: result = op1 ^ op2;
      VALU_OP_ROT: result = 128'hADE01234;
      VALU_OP_AES_SUBBYTES : result = 128'hDEADBEEF;
      VALU_OP_AES_INV_SUBBYTES : result = 128'hDEADBEEF;
      VALU_OP_AES_SHIFTROWS: result = 128'hBADFA550;
      VALU_OP_AES_MIX_COLUMNS: result = 128'h1BADBABE;
      VALU_OP_AES_KEYSCHE_XOR: result = 128'hAABAAABA;
      default: result = 0;  // div no implementada aun 
    endcase
  end
endmodule
