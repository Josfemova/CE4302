`timescale 1ns / 1ps

module stage_execute_tb;

    // Parametros
    parameter WIDTH = 128;

    // Entradas 
    reg clk;
    reg reset;
    reg mem_clear;
    reg mem_stall;
    reg [31:0] ex_instr;
    reg ex_reg_write;
    reg ex_mem_write;
    reg ex_mem_read;
    reg ex_jump;
    reg ex_jump_cond;
    reg [2:0] ex_jump_cond_type;
    reg [3:0] ex_alu_control;
    reg ex_alu_src_op1;
    reg ex_alu_src_op2;
    reg ex_pc_target_src;
    reg [1:0] ex_result_src;
    reg ex_vector_op;
    reg [31:0] ex_pc;
    reg [31:0] ex_pc_plus_4;
    reg [31:0] ex_imm_ext;
    reg [127:0] ex_rd1;
    reg [127:0] ex_rd2;
    reg [4:0] ex_rd;
    reg [127:0] wb_result;
    reg [1:0] ex_op1_forward;
    reg [1:0] ex_op2_forward;

    // Salidas 
    wire [31:0] mem_instr;
    wire mem_reg_write;
    wire mem_mem_write;
    wire mem_mem_read;
    wire [1:0] mem_result_src;
    wire mem_vector_op;
    wire [127:0] mem_alu_result;
    wire [127:0] mem_write_data;
    wire [31:0] mem_pc_plus_4;
    wire [127:0] mem_imm_ext;
    wire [4:0] mem_rd;
    wire ex_pc_src;
    wire [31:0] ex_pc_target;

    stage_execute uut (
        .clk(clk),
        .reset(reset),
        .mem_clear(mem_clear),
        .mem_stall(mem_stall),
        .ex_instr(ex_instr),
        .ex_reg_write(ex_reg_write),
        .ex_mem_write(ex_mem_write),
        .ex_mem_read(ex_mem_read),
        .ex_jump(ex_jump),
        .ex_jump_cond(ex_jump_cond),
        .ex_jump_cond_type(ex_jump_cond_type),
        .ex_alu_control(ex_alu_control),
        .ex_alu_src_op1(ex_alu_src_op1),
        .ex_alu_src_op2(ex_alu_src_op2),
        .ex_pc_target_src(ex_pc_target_src),
        .ex_result_src(ex_result_src),
        .ex_vector_op(ex_vector_op),
        .ex_pc(ex_pc),
        .ex_pc_plus_4(ex_pc_plus_4),
        .ex_imm_ext(ex_imm_ext),
        .ex_rd1(ex_rd1),
        .ex_rd2(ex_rd2),
        .ex_rd(ex_rd),
        .wb_result(wb_result),
        .ex_op1_forward(ex_op1_forward),
        .ex_op2_forward(ex_op2_forward),
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
        .ex_pc_src(ex_pc_src),
        .ex_pc_target(ex_pc_target)
    );

    // Generador de reloj
    always begin
        #5 clk = ~clk; // Reloj con periodo de 10 ns
    end

    // Proceso inicial
    initial begin

        clk = 0;
        reset = 1;
        mem_clear = 1;
        mem_stall = 0;

        // Configurar las entradas
        ex_instr = 32'h00000000;
        ex_reg_write = 0;
        ex_mem_write = 0;
        ex_mem_read = 0;
        ex_jump = 0;
        ex_jump_cond = 0;
        ex_jump_cond_type = 3'b000;
        ex_alu_control = 4'b0000;
        ex_alu_src_op1 = 0;
        ex_alu_src_op2 = 0;
        ex_pc_target_src = 0;
        ex_result_src = 2'b00;
        ex_vector_op = 0;
        ex_pc = 32'h00000000;
        ex_pc_plus_4 = 32'h00000004;
        ex_imm_ext = 32'h00000000;
        ex_rd1 = 128'h00000000000000000000000000000000;
        ex_rd2 = 128'h00000000000000000000000000000000;
        ex_rd = 5'b00000;
        wb_result = 128'h00000000000000000000000000000000;
        ex_op1_forward = 2'b00;
        ex_op2_forward = 2'b00;

        // Liberar el reset despues de 20 ns
        #20 reset = 0;

        #10 ex_instr = 32'h00000001; // Primera instrucción
        ex_reg_write = 1; ex_mem_write = 1; ex_mem_read = 0; // Configurar para escribir
        #10 ex_rd1 = 128'hFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF; // Datos de entrada
        #10 ex_rd2 = 128'hAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA; // Datos de entrada
        ex_op1_forward = 2'b01; // Hacia wb_result
        ex_op2_forward = 2'b00; // Hacia ex_rd2
        #10;

        // Limpiar memoria
        mem_clear = 0;
        #10 mem_clear = 1;

        #50 $finish;
    end

    // Monitorear señales
    initial begin
        $monitor("Time: %0t | ex_instr: %h | mem_reg_write: %b | mem_mem_write: %b | mem_mem_read: %b | mem_alu_result: %h", 
                  $time, ex_instr, mem_reg_write, mem_mem_write, mem_mem_read, mem_alu_result);
    end

endmodule
