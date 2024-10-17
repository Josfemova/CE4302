module stage_decode (
    input clk,
    input ex_clear,
    input ex_stall, 

    // inputs de stage previo
    input [31:0] de_instr,
    input [31:0] de_pc,
    input [31:0] de_pc_plus4,

    // inputs de stage de writeback
    input [127:0] wb_result,
    input wb_reg_write,
    input [4:0] wb_rd,
    
    // debug 
    output reg [31:0] ex_instr,

    // outputs de control unit
    output reg ex_reg_write,
    output reg ex_mem_write, // avalon write
    output reg ex_mem_read, // avalon read
    output reg ex_jump,
    output reg ex_jump_cond,
    output reg [2:0] ex_jump_cond_type,
    output reg [3:0] ex_alu_control,
    output reg ex_alu_src_op1,
    output reg ex_alu_src_op2,
    output reg ex_pc_target_src,
    output reg [1:0] ex_result_src,
    output reg ex_vector_op,

    // outputs del data path
    output reg [31:0] ex_pc,
    output reg [31:0] ex_pc_plus_4,
    output reg [31:0] ex_imm_ext,
    output reg [127:0] ex_rd1,
    output reg [127:0] ex_rd2,

    output reg [4:0] ex_rd,
    output reg [4:0] ex_rs1,
    output reg [4:0] ex_rs2,

    // outputs que usa hazard unit
    output [4:0] de_rs1,
    output [4:0] de_rs2
);


  // señales internas del stage
  logic [3:0] imm_src;
  logic [127:0] rd1;
  logic [127:0] rd2;
  logic [31:0] imm_out;
  logic reg_write;
  logic mem_write;
  logic mem_read;
  logic jump;
  logic jump_cond;
  logic alu_src_op1;
  logic alu_src_op2;
  logic pc_target_src;
  logic [1:0] result_src;
  logic [2:0] jump_cond_type;
  logic [3:0] alu_control;
  logic vector_op;

  logic [4:0] rs1;
  logic [4:0] rs2;
  logic [4:0] rd;

  assign rs1 = de_instr[15:11];
  assign rs2 = de_instr[20:16];
  assign rd  = de_instr[10:6];

  register_file reg_file (
      .clk(~clk),  // recibe clk negado
      .we3(wb_reg_write),
      .a1(rs1),
      .a2(rs2),
      .a3(wb_rd),
      .wd3(wb_result),
      // outputs
      .rd1(rd1),
      .rd2(rd2)
  );

  control_unit ctrl_unit (
      .op(de_instr[2:0]),
      .func3(de_instr[5:3]),
      .func11(de_instr[31:21]),

      // outputs
      .reg_write(reg_write),
      .mem_write(mem_write),
      .mem_read(mem_read),
      .jump(jump),
      .jump_cond(jump_cond),
      .jump_cond_type(jump_cond_type),
      .alu_control(alu_control),
      .alu_src_op1(alu_src_op1),
      .alu_src_op2(alu_src_op2),
      .pc_target_src(pc_target_src),
      .imm_src(imm_src),
      .result_src(result_src),
      .vector_op(vector_op)
  );

  imm_extend imm_ext (
      .imm_in (de_instr[31:6]),
      .imm_src(imm_src),
      //output
      .imm_out(imm_out)
  );

  assign de_rs1 = rs1;
  assign de_rs2 = rs2;

  always @(posedge clk) begin
    if (ex_clear) begin
      // debug 
      ex_instr <= 0;
      // outputs de control unit
      ex_reg_write      <= 0;
      ex_mem_write      <= 0;
      ex_mem_read       <= 0;
      ex_jump           <= 0;
      ex_jump_cond      <= 0;
      ex_jump_cond_type <= 0;
      ex_alu_control    <= 0;
      ex_alu_src_op1    <= 0;
      ex_alu_src_op2    <= 0;
      ex_pc_target_src  <= 0;
      ex_result_src     <= 0;
      ex_vector_op      <= 0;

      // outputs del data path
      ex_pc             <= 0;
      ex_pc_plus_4      <= 0;
      ex_imm_ext        <= 0;
      ex_rd1            <= 0;
      ex_rd2            <= 0;
      ex_rd             <= 0;
      ex_rs1            <= 0;
      ex_rs2            <= 0;
    end else if(~ex_stall) begin
      // debug 
      ex_instr <= de_instr;

      // outputs de control unit
      ex_reg_write      <= reg_write;
      ex_mem_write      <= mem_write;
      ex_mem_read       <= mem_read;
      ex_jump           <= jump;
      ex_jump_cond      <= jump_cond;
      ex_jump_cond_type <= jump_cond_type;
      ex_alu_control    <= alu_control;
      ex_alu_src_op1    <= alu_src_op1;
      ex_alu_src_op2    <= alu_src_op2;
      ex_pc_target_src  <= pc_target_src;
      ex_result_src     <= result_src;
      ex_vector_op      <= vector_op;

      // outputs del data path
      ex_pc             <= de_pc;
      ex_pc_plus_4      <= de_pc_plus4;
      ex_imm_ext        <= imm_out;
      ex_rd1            <= rd1;
      ex_rd2            <= rd2;
      ex_rd             <= rd;
      ex_rs1            <= rs1;
      ex_rs2            <= rs2;
    end
  end


endmodule
