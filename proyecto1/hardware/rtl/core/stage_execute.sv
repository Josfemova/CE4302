module stage_execute (
    input clk,
    input reset, 
    input mem_clear,
    input mem_stall,
    // debug
    input [31:0] ex_instr, 
    // inputs de control unit
    input ex_reg_write,
    input ex_mem_write,
    input ex_mem_read,
    input ex_jump,
    input ex_jump_cond,
    input [2:0] ex_jump_cond_type,
    input [3:0] ex_alu_control,
    input ex_alu_src_op1,
    input ex_alu_src_op2,
    input ex_pc_target_src,
    input [1:0] ex_result_src,
    input ex_vector_op,

    // inputs del data path
    input [31:0] ex_pc,
    input [31:0] ex_pc_plus_4,
    input [31:0] ex_imm_ext,
    input [127:0] ex_rd1,
    input [127:0] ex_rd2,

    input [4:0] ex_rd,

    // inputs de otras etapas
    input [127:0] wb_result,

    // inputs de hazard unit
    input [1:0] ex_op1_forward,
    input [1:0] ex_op2_forward,

    // debug
    output reg [31:0] mem_instr,
    // outputs de control unit
    output reg mem_reg_write,
    output reg mem_mem_write,
    output reg mem_mem_read,
    output reg [1:0] mem_result_src,
    output reg mem_vector_op,
    // outputs del data path
    output reg [127:0] mem_alu_result,
    output reg [127:0] mem_write_data,
    output reg [31:0] mem_pc_plus_4,
    output reg [127:0] mem_imm_ext,
    output reg [ 4:0] mem_rd,

    // output a otras etapas que no son la inmediatamente siguiente
    // y dependen de lógica combinacional
    // output para la hazard unit
    output ex_pc_src,
    // posible pc target 
    output [31:0] ex_pc_target

);


  // señales internas del stage
  logic [127:0] pre_op1;
  logic [127:0] op1;
  logic [127:0] write_data;
  logic [127:0] op2;
  logic [3:0] alu_flags;
  logic jump_cond_true;
  logic alu_sel;
  logic [127:0] alu_result;
  logic [31:0] scal_alu_result;
  logic [127:0] vector_alu_result;
  reg [127:0] mem_alu_result_proxy;
  
  always @(*) begin
    case (ex_op1_forward)
      2'b01:   pre_op1 = wb_result;
      2'b10:   pre_op1 = mem_alu_result_proxy;
      default: pre_op1 = ex_rd1;
    endcase

    case (ex_op2_forward)
      2'b01:   write_data = wb_result;
      2'b10:   write_data = {4{mem_alu_result_proxy[31:0]}};
      default: write_data = ex_rd2;
    endcase
  end


  jump_cond_ctrl cond_unit (
      .jump_cond(ex_jump_cond),
      .jump_cond_type(ex_jump_cond_type),
      .alu_flags(alu_flags),
      .jump_cond_true(jump_cond_true)
  );

  //! agregar vector op aca
  alu alu0 (
      .op1(op1[31:0]),
      .op2(op2[31:0]),
      .alu_control(ex_alu_control),
      .flags(alu_flags),
      .result(scal_alu_result)
  );

  vector_alu valu0 (
      .op1(op1),
      .op2(op2),
      .alu_control(ex_alu_control),
      .result(vector_alu_result)
  );
  
  // seleccionar escalar si la OP no es vectorial o es de memoria (alu calcula addr)
  assign alu_sel = (~ex_vector_op) | ex_mem_read | ex_mem_write; 
  assign alu_result = (alu_sel) ? {4{scal_alu_result}} : vector_alu_result;

  assign op1 = (ex_alu_src_op1) ? pre_op1 : 127'b0;
  assign op2 = (ex_alu_src_op2) ? ex_imm_ext : write_data;

  assign mem_alu_result = mem_alu_result_proxy;
 
  assign ex_pc_target = (ex_pc_target_src)? alu_result[31:0] : ex_pc + ex_imm_ext[31:0];
  assign ex_pc_src = ((ex_jump_cond & jump_cond_true) | (ex_jump)) & (~reset);

  always @(posedge clk) begin
    if (mem_clear) begin
      //debug
      mem_instr <= 0;
      // outputs de control unit
      mem_reg_write        <= 0;
      mem_mem_write        <= 0;
      mem_mem_read         <= 0;
      mem_result_src       <= 0;
      mem_vector_op        <= 0;

      // outputs del data path
      mem_alu_result_proxy <= 0;
      mem_write_data       <= 0;
      mem_pc_plus_4        <= 0;
      mem_imm_ext          <= 0;
      mem_rd               <= 0;
    end else if(~mem_stall) begin
      //debug
      mem_instr <= ex_instr;
      // outputs de control unit
      mem_reg_write        <= ex_reg_write;
      mem_mem_write        <= ex_mem_write;
      mem_mem_read         <= ex_mem_read;
      mem_result_src       <= ex_result_src;
      mem_vector_op        <= ex_vector_op;

      // outputs del data path
      mem_alu_result_proxy <= alu_result;
      mem_write_data       <= write_data;
      mem_pc_plus_4        <= ex_pc_plus_4;
      mem_imm_ext          <= {4{ex_imm_ext}};
      mem_rd               <= ex_rd;
    end
  end

endmodule
