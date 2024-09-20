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

  wire [127:0] xor_res;
  wire [127:0] vrot_res;
  wire [127:0] subb_res;
  wire [127:0] isubb_res;
  wire [127:0] shiftrows_res;
  wire [127:0] mixcols_res;
  wire [127:0] kschxor_res;

logic [127:0] opA;
  
  add_round_key xorinst(.state(opA), .round_key(op2), .new_state(xor_res));
  vrot32 vrot32inst (.state_in(op1), .bits_to_rotate(op2), .state_out(vrot_res));
  sub_bytes subbinst(.state_in(op1), .state_out(subb_res));
  inv_sub_bytes isubbinst(.state_in(op1), .state_out(isubb_res));
  shift_rows srinst(.state_in(op1), .state_out(shiftrows_res));
  mix_columns mixcols(.state_in(op1), .state_out(mixcols_res));
  key_schedule kschxor(.K(op1), .result(op2[31:0]), .round_key(kschxor_res));


  always @(*) begin
    opA = op1;
    case (alu_control)
      VALU_OP_XOR: 
          result = xor_res;
      VALU_OP_ROT: 
          result = vrot_res;
      VALU_OP_AES_SUBBYTES : 
        begin
          opA = subb_res;
          result = xor_res;
        end
      VALU_OP_AES_INV_SUBBYTES : 
        begin
          opA = isubb_res;
          result = xor_res;
        end
      VALU_OP_AES_SHIFTROWS: 
        begin
          opA = shiftrows_res;
          result = xor_res;
        end
      VALU_OP_AES_MIX_COLUMNS: 
        begin
          opA = mixcols_res;
          result = xor_res;
        end
      VALU_OP_AES_KEYSCHE_XOR: 
        begin
          result = kschxor_res;
        end
      default: result = 0;  // div no implementada aun 
    endcase
  end
endmodule
