module stage_writeback_tb;

    // Entradas
    reg [1:0] wb_result_src;
    reg wb_vector_op;
    reg [127:0] wb_alu_result;
    reg [127:0] wb_read_result;
    reg [31:0] wb_pc_plus_4;
    reg [127:0] wb_imm_ext;

    // Salida
    wire [127:0] wb_result;

    stage_writeback dut (
        .wb_result_src(wb_result_src),
        .wb_vector_op(wb_vector_op),
        .wb_alu_result(wb_alu_result),
        .wb_read_result(wb_read_result),
        .wb_pc_plus_4(wb_pc_plus_4),
        .wb_imm_ext(wb_imm_ext),
        .wb_result(wb_result)
    );

    initial begin

        wb_result_src = 2'b00;
        wb_vector_op = 0;
        wb_alu_result = 128'hA5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5;
        wb_read_result = 128'h5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A5A;
        wb_pc_plus_4 = 32'h00000004;
        wb_imm_ext = 128'hFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF;

        #10 wb_result_src = 2'b01;  // Seleccionar wb_read_result
        #10 wb_vector_op = 1;        // Activar modo vectorial
        #10 wb_result_src = 2'b10;   // Seleccionar wb_pc_plus_4
        #10 wb_result_src = 2'b11;   // Seleccionar wb_imm_ext
        #10 wb_result_src = 2'b00;   // Seleccionar wb_alu_result

        #10 $finish;
    end

    // Monitoreo de resultados
    initial begin
        $monitor("Time: %0t | wb_result_src: %b | wb_vector_op: %b | wb_result: %h", 
                 $time, wb_result_src, wb_vector_op, wb_result);
    end

endmodule
