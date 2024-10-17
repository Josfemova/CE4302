module stage_decode_tb;
    reg clk;
    reg ex_clear;
    reg ex_stall;
    reg stall_all;

    // Inputs de stage previo
    reg [31:0] de_instr;
    reg [31:0] de_pc;
    reg [31:0] de_pc_plus4;

    // Inputs de stage de writeback
    reg [127:0] wb_result;
    reg wb_reg_write;
    reg [4:0] wb_rd;
    
    // Outputs
    wire [31:0] ex_instr;
    wire ex_reg_write;
    wire ex_mem_write;
    wire ex_mem_read;
    wire ex_jump;
    wire ex_jump_cond;
    wire [2:0] ex_jump_cond_type;
    wire [3:0] ex_alu_control;
    wire ex_alu_src_op1;
    wire ex_alu_src_op2;
    wire ex_pc_target_src;
    wire [1:0] ex_result_src;
    wire ex_vector_op;
    wire [31:0] ex_pc;
    wire [31:0] ex_pc_plus_4;
    wire [31:0] ex_imm_ext;
    wire [127:0] ex_rd1;
    wire [127:0] ex_rd2;
    wire [4:0] ex_rd;
    wire [4:0] ex_rs1;
    wire [4:0] ex_rs2;
    wire [4:0] de_rs1;
    wire [4:0] de_rs2;

    stage_decode uut (
        .clk(clk),
        .ex_clear(ex_clear),
        .ex_stall(ex_stall),
        .stall_all(stall_all),
        .de_instr(de_instr),
        .de_pc(de_pc),
        .de_pc_plus4(de_pc_plus4),
        .wb_result(wb_result),
        .wb_reg_write(wb_reg_write),
        .wb_rd(wb_rd),
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
        .ex_rs1(ex_rs1),
        .ex_rs2(ex_rs2),
        .de_rs1(de_rs1),
        .de_rs2(de_rs2)
    );

    initial begin

        clk = 0;
        ex_clear = 1;
        ex_stall = 0;
        stall_all = 0;
        de_instr = 32'b0;
        de_pc = 32'b0;
        de_pc_plus4 = 32'b0;
        wb_result = 128'b0;
        wb_reg_write = 0;
        wb_rd = 5'b0;

        $monitor("Time: %0t | de_instr: %h | de_pc: %h | de_pc_plus4: %h | wb_result: %h | wb_reg_write: %b | wb_rd: %b | ex_instr: %h | ex_reg_write: %b | ex_mem_write: %b | ex_mem_read: %b | ex_jump: %b | ex_pc: %h", 
                 $time, de_instr, de_pc, de_pc_plus4, wb_result, wb_reg_write, wb_rd, ex_instr, ex_reg_write, ex_mem_write, ex_mem_read, ex_jump, ex_pc);
        
        // Test 1: Probar instrucciones normales
        #5;
        ex_clear = 0;
        de_instr = 32'b00000000000100000000000000000000; // Ejemplo de instruccion
        de_pc = 32'h00000004;
        de_pc_plus4 = 32'h00000008;
        wb_result = 128'hDEADBEEFDEADBEEFDEADBEEFDEADBEEF; // Resultado simulado
        wb_reg_write = 1; // Habilitar escritura
        wb_rd = 5'b00001; // Registro de destino
        #10;

        // Test 2: Probar con stall
        ex_stall = 1; // Activar stall
        #5;

        // Test 3: Limpiar el registro
        ex_stall = 0; // Desactivar stall
        ex_clear = 1; // Activar limpieza
        #5;
        
        $finish;
    end

    // Generar reloj
    always #5 clk = ~clk;

endmodule
