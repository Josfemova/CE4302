module stage_instruction_fetch_tb;
  reg clk;
  reg reset;
  reg [31:0] reset_vector_addr;
  reg de_clear;
  reg if_stall;
  reg de_stall;
  reg ex_pc_src;
  reg [31:0] ex_pc_target;
  reg [31:0] if_instr_rd;
  wire [31:0] if_pc_next_instr_mem;
  wire [31:0] de_instr;
  wire [31:0] de_pc;
  wire [31:0] de_pc_plus4;

  stage_instruction_fetch dut (
    .clk(clk),
    .reset(reset),
    .reset_vector_addr(reset_vector_addr),
    .de_clear(de_clear),
    .if_stall(if_stall),
    .de_stall(de_stall),
    .ex_pc_src(ex_pc_src),
    .ex_pc_target(ex_pc_target),
    .if_instr_rd(if_instr_rd),
    .if_pc_next_instr_mem(if_pc_next_instr_mem),
    .de_instr(de_instr),
    .de_pc(de_pc),
    .de_pc_plus4(de_pc_plus4)
  );

  initial begin
    clk = 0;
    forever #5 clk = ~clk;
  end

  initial begin
    reset = 1; reset_vector_addr = 32'h0000_0000; if_instr_rd = 32'hDEADBEEF; 
    #10 reset = 0; de_clear = 0; if_stall = 0; de_stall = 0; ex_pc_src = 0; ex_pc_target = 32'h0000_0004;

    // Pruebas
    #10; // Normal operation
    #10 de_clear = 1; #10 de_clear = 0; // Clear DE
    #10 if_stall = 1; #10 if_stall = 0; // Stall IF
    #10 ex_pc_src = 1; // Change to EX target

    $finish;
  end

  initial begin
    $monitor("Time: %0t | Reset: %b | IF Stalling: %b | DE Stalling: %b | PC Next Instr Mem: %h | DE Instr: %h | DE PC: %h", 
             $time, reset, if_stall, de_stall, if_pc_next_instr_mem, de_instr, de_pc);
  end
endmodule
