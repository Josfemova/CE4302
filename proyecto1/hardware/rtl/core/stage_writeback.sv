module stage_writeback(

    input [1:0] wb_result_src,
    input wb_vector_op,
    // inputs del data path
    input [127:0] wb_alu_result,
    input [127:0] wb_read_result,
    input [31:0] wb_pc_plus_4,
    input [127:0] wb_imm_ext,
    // única salida
    output [127:0] wb_result
);

reg [127:0] pre_wb_result;

// mantener mismo comportamiento que lectura de reg file
assign wb_result = (wb_vector_op) ? pre_wb_result : {4{pre_wb_result[31:0]}};

always @(*) begin
    case (wb_result_src)
        2'b01: pre_wb_result =  wb_read_result;
        2'b10: pre_wb_result =  wb_pc_plus_4;
        2'b11: pre_wb_result = wb_imm_ext;
        default: pre_wb_result = wb_alu_result;
    endcase
end


endmodule