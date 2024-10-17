module hazard_unit (
    input wire reset,
    input wire [4:0] de_rs1,
    input wire [4:0] de_rs2,
    input wire [4:0] ex_rs1,
    input wire [4:0] ex_rs2,
    input wire [4:0] ex_rd,
    input wire ex_pc_src,
    input wire [1:0] ex_result_src,
    input wire [4:0] mem_rd,
    input wire mem_reg_write,
    input wire [4:0] wb_rd,
    input wire wb_reg_write,
    input wire stall_all,

    output wire if_stall,
    output wire de_stall,
    output wire ex_stall,
    output wire mem_stall,
    output wire wb_stall,
    output wire de_flush,
    output wire ex_flush,
    output logic [1:0] ex_op1_forward,
    output logic [1:0] ex_op2_forward
);

  // a diferencia del RV del libro, 2b11 corresponde al valor del imm, entonces si se chequea 
  // igualdad a 2'b01 que selecciona la salida del dato leído de memoria. 
  wire ldm_hazard_stall = (ex_result_src == 2'b01) & ((de_rs1 == ex_rd) | (de_rs2 == ex_rd));
  assign if_stall = stall_all | ldm_hazard_stall | reset;
  assign de_stall = stall_all | ldm_hazard_stall | reset;
  assign ex_stall =  stall_all | reset;
  assign mem_stall = ex_stall;
  assign wb_stall = ex_stall;
  assign de_flush = ex_pc_src | reset;
  assign ex_flush = ldm_hazard_stall | ex_pc_src | reset;

  always @(*) begin
    // adelantamiento de rs1
    if (((ex_rs1 == mem_rd) & mem_reg_write) & (ex_rs1 != 0)) begin
      ex_op1_forward = 2'b10;
    end else if (((ex_rs1 == wb_rd) & wb_reg_write) & (ex_rs1 != 0)) begin
      ex_op1_forward = 2'b01;
    end else begin
      ex_op1_forward = 2'b00;
    end
    // adelantamiento de rs2
    if (((ex_rs2 == mem_rd) & mem_reg_write) & (ex_rs2 != 0)) begin
      ex_op2_forward = 2'b10;
    end else if (((ex_rs2 == wb_rd) & wb_reg_write) & (ex_rs2 != 0)) begin
      ex_op2_forward = 2'b01;
    end else begin
      ex_op2_forward = 2'b00;
    end
  end

endmodule
