module stage_memory_tb;

  // Entradas
  reg clk;
  reg reset;
  reg wb_clear;
  reg wb_stall;
  reg [31:0] mem_instr;
  reg mem_reg_write;
  reg mem_mem_write;
  reg mem_mem_read;
  reg [1:0] mem_result_src;
  reg mem_vector_op;
  reg [127:0] mem_alu_result;
  reg [127:0] mem_write_data;
  reg [31:0] mem_pc_plus_4;
  reg [127:0] mem_imm_ext;
  reg [4:0] mem_rd;
  reg [31:0] mem_read_result;

  // Salidas
  wire [31:0] wb_instr;
  wire [31:0] mem_data_memory_addr;
  wire [31:0] mem_data_memory_writedata;
  wire mem_stall_all;
  wire wb_reg_write;
  wire [1:0] wb_result_src;
  wire wb_vector_op;
  wire [127:0] wb_alu_result;
  wire [127:0] wb_read_result;
  wire [31:0] wb_pc_plus_4;
  wire [127:0] wb_imm_ext;
  wire [4:0] wb_rd;

  stage_memory uut (
    .clk(clk),
    .reset(reset),
    .wb_clear(wb_clear),
    .wb_stall(wb_stall),
    .mem_instr(mem_instr),
    .mem_reg_write(mem_reg_write),
    .mem_mem_write(mem_mem_write),
    .mem_mem_read(mem_mem_read),
    .mem_result_src(mem_result_src),
    .mem_vector_op(mem_vector_op),
    .mem_alu_result(mem_alu_result),
    .mem_write_data(mem_write_data),
    .mem_pc_plus_4(mem_pc_plus_4),
    .mem_imm_ext(mem_imm_ext),
    .mem_rd(mem_rd),
    .mem_read_result(mem_read_result),
    .wb_instr(wb_instr),
    .mem_data_memory_addr(mem_data_memory_addr),
    .mem_data_memory_writedata(mem_data_memory_writedata),
    .mem_stall_all(mem_stall_all),
    .wb_reg_write(wb_reg_write),
    .wb_result_src(wb_result_src),
    .wb_vector_op(wb_vector_op),
    .wb_alu_result(wb_alu_result),
    .wb_read_result(wb_read_result),
    .wb_pc_plus_4(wb_pc_plus_4),
    .wb_imm_ext(wb_imm_ext),
    .wb_rd(wb_rd)
  );

  // Generador de reloj
  initial begin
    clk = 0;
    forever #5 clk = ~clk; // Cambiar de estado cada 5 unidades de tiempo
  end

  // Testbench
  initial begin

    reset = 1;
    wb_clear = 1;
    wb_stall = 0;
    mem_instr = 32'hDEADBEEF;
    mem_reg_write = 1;
    mem_mem_write = 1;
    mem_mem_read = 1;
    mem_result_src = 2'b01;
    mem_vector_op = 1;
    mem_alu_result = 128'h1234567890ABCDEF1234567890ABCDEF;
    mem_write_data = 128'hDEADBEEFDEADBEEFDEADBEEFDEADBEEF;
    mem_pc_plus_4 = 32'h00000004;
    mem_imm_ext = 128'h00000000000000000000000000000001;
    mem_rd = 5'b00001;
    mem_read_result = 32'hBAADF00D;

    // Ciclos de simulacion
    #10 reset = 0; // Desactivar reset
    #10 wb_clear = 0; // Desactivar wb_clear
    #10 wb_stall = 1; // Activar wb_stall
    #10 wb_stall = 0; // Desactivar wb_stall
    #10; // Esperar un ciclo más

    #50 $finish;
  end

  initial begin
    $monitor("Time: %0t | wb_instr: %h | wb_reg_write: %b | wb_result_src: %b | wb_vector_op: %b | wb_alu_result: %h | wb_read_result: %h | wb_pc_plus_4: %h | wb_imm_ext: %h | wb_rd: %b",
             $time, wb_instr, wb_reg_write, wb_result_src, wb_vector_op, wb_alu_result, wb_read_result, wb_pc_plus_4, wb_imm_ext, wb_rd);
  end

endmodule
