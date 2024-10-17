module stage_memory (
    input clk,
    input reset,
    input wb_clear,
    input wb_stall,
    // debug
    input [31:0] mem_instr,

    // inputs de control unit
    input mem_reg_write,
    //input mem_mem_write,
    //input mem_mem_read,
    input [1:0] mem_result_src,
    input mem_vector_op,

    // inputs del data path
    input [127:0] mem_alu_result,
    input [127:0] mem_write_data,
    input [31:0] mem_pc_plus_4,
    input [127:0] mem_imm_ext,
    input [ 4:0] mem_rd,

    // input de la memoria de datos, es del ancho del puerto de lectura, no 
    // el ancho del datapath
    input [31:0] mem_read_result,

    // debug 
    output reg [31:0] wb_instr,

    // control de la memoria 
    output reg [31:0] mem_data_memory_addr,
    output reg [31:0] mem_data_memory_writedata, //ancho de mem, no de dato
    output mem_stall_all,
    
    // outputs de control unit
    output reg wb_reg_write,
    output reg [1:0] wb_result_src,
    output reg wb_vector_op,

    // outputs del data path
    output reg [127:0] wb_alu_result,
    output reg [127:0] wb_read_result,
    output reg [31:0] wb_pc_plus_4,
    output reg [127:0] wb_imm_ext,
    output reg [ 4:0] wb_rd
);
  
  wire [127:0] temp_read_proxy;

  load_store_unit ldstu(
    .clk, 
    .reset(reset),
    .vector_op(mem_vector_op),
    .base_addr(mem_alu_result[31:0]), // ignorar bits altos
    .in_writedata(mem_write_data), 
    .in_readdata(mem_read_result), 
    .stall(mem_stall_all), // when asserted low, value is ready
    .current_mem_addr(mem_data_memory_addr),
    // memoria sync
    .out_readdata(wb_read_result), // se ve en wb, no mem
    .out_writedata(mem_data_memory_writedata)
  );
  
  always @(posedge clk) begin
    if (wb_clear) begin
      wb_reg_write <= 0;
      wb_result_src <= 0;
      wb_vector_op <= 0;

      wb_alu_result <= 0;
      wb_pc_plus_4 <= 0;
      wb_imm_ext <= 0;
      wb_rd <= 0;
    end else if(~wb_stall) begin
      //debug 
      wb_instr <= mem_instr;

      wb_reg_write <= mem_reg_write;
      wb_result_src <= mem_result_src;
      wb_vector_op <= mem_vector_op;
      
      wb_alu_result <= mem_alu_result;
      wb_pc_plus_4 <= mem_pc_plus_4;
      wb_imm_ext <= mem_imm_ext;
      wb_rd <= mem_rd;
    end
  end



endmodule
