module stage_instruction_fetch (
    input clk,
    input reset,
    input [31:0] reset_vector_addr,//selecciona direccion donde empieza a ejecutar
    input de_clear,
    input if_stall,
    input de_stall,
    input ex_pc_src,  // selecciona pc source 
    input [31:0] ex_pc_target,
    input [31:0] if_instr_rd,  // de la memoria rom 
    output [31:0] if_pc_next_instr_mem,  // a addr de rom
    // outputs para siguiente stage
    output reg [31:0] de_instr,  // a la memoria rom y a reg de decode
    output reg [31:0] de_pc,
    output reg [31:0] de_pc_plus4

);

  logic [31:0] if_pc_plus4;
  logic [31:0] if_pc_next;
  logic [31:0] if_pc;

  assign if_pc_plus4 = if_pc + 4;
  assign if_pc_next = (ex_pc_src == 0) ? if_pc_plus4 : ex_pc_target;
  assign if_pc_next_instr_mem = (reset) ? reset_vector_addr : if_pc_next;

  always @(posedge clk) begin
    if (reset) if_pc <= -4;
    else if (~if_stall) if_pc <= if_pc_next;
  end

  // señales de siguiente etapa
  // solo rutear la info que viene de la rom, la rom recibe de_stall y de_flush
  always @(posedge clk) begin
    if (de_clear) begin
      de_instr <= 0;
      de_pc <= 0;
      de_pc_plus4 <= 0;
    end else if (~de_stall) begin
      de_instr = if_instr_rd; // kaka
      de_pc <= if_pc;
      de_pc_plus4 <= if_pc_plus4;
    end
  end


endmodule
