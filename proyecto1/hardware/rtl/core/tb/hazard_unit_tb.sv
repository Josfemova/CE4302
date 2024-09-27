module hazard_unit_tb;
  reg reset;
  reg [4:0] de_rs1;
  reg [4:0] de_rs2;
  reg [4:0] ex_rs1;
  reg [4:0] ex_rs2;
  reg [4:0] ex_rd;
  reg ex_pc_src;
  reg [1:0] ex_result_src;
  reg [4:0] mem_rd;
  reg mem_reg_write;
  reg [4:0] wb_rd;
  reg wb_reg_write;

  wire if_stall;
  wire de_stall;
  wire de_flush;
  wire ex_flush;
  wire [1:0] ex_op1_forward;
  wire [1:0] ex_op2_forward;

  hazard_unit hu (
    .reset(reset),
    .de_rs1(de_rs1),
    .de_rs2(de_rs2),
    .ex_rs1(ex_rs1),
    .ex_rs2(ex_rs2),
    .ex_rd(ex_rd),
    .ex_pc_src(ex_pc_src),
    .ex_result_src(ex_result_src),
    .mem_rd(mem_rd),
    .mem_reg_write(mem_reg_write),
    .wb_rd(wb_rd),
    .wb_reg_write(wb_reg_write),
    .if_stall(if_stall),
    .de_stall(de_stall),
    .de_flush(de_flush),
    .ex_flush(ex_flush),
    .ex_op1_forward(ex_op1_forward),
    .ex_op2_forward(ex_op2_forward)
  );

   initial begin
    $monitor("Time: %0t | reset: %b | de_rs1: %0d | de_rs2: %0d | ex_rs1: %0d | ex_rs2: %0d | ex_rd: %0d | ex_pc_src: %b | ex_result_src: %b | mem_rd: %0d | mem_reg_write: %b | wb_rd: %0d | wb_reg_write: %b | if_stall: %b | de_stall: %b | de_flush: %b | ex_flush: %b | ex_op1_forward: %b | ex_op2_forward: %b",
              $time, reset, de_rs1, de_rs2, ex_rs1, ex_rs2, ex_rd, ex_pc_src, ex_result_src, mem_rd, mem_reg_write, wb_rd, wb_reg_write, if_stall, de_stall, de_flush, ex_flush, ex_op1_forward, ex_op2_forward);
   end

  
  initial begin
    // No hazards
    reset = 0;
    de_rs1 = 0;
    de_rs2 = 0;
    ex_rs1 = 0;
    ex_rs2 = 0;
    ex_rd = 0;
    ex_pc_src = 0;
    ex_result_src = 2'b00;
    mem_rd = 0;
    mem_reg_write = 0;
    wb_rd = 0;
    wb_reg_write = 0;
    #10;
    
    // Data hazard with forwarding (rs1)
    reset = 0;
    de_rs1 = 3'b010;
    de_rs2 = 0;
    ex_rs1 = 3'b010;
    ex_rs2 = 0;
    ex_rd = 0;
    ex_pc_src = 0;
    ex_result_src = 2'b00;
    mem_rd = 0;
    mem_reg_write = 0;
    wb_rd = 0;
    wb_reg_write = 0;
    #10;
    
    // Data hazard with forwarding (rs2)
    reset = 0;
    de_rs1 = 0;
    de_rs2 = 3'b010;
    ex_rs1 = 0;
    ex_rs2 = 3'b010;
    ex_rd = 0;
    ex_pc_src = 0;
    ex_result_src = 2'b00;
    mem_rd = 0;
    mem_reg_write = 0;
    wb_rd = 0;
    wb_reg_write = 0;
    #10;
   
	// Load-Use Data Hazard without forwarding
	reset = 0;
	de_rs1 = 3'b010; // Load
	de_rs2 = 0;
	ex_rs1 = 0;
	ex_rs2 = 0;
	ex_rd = 0;
	ex_pc_src = 0;
	ex_result_src = 2'b00;
	mem_rd = 3'b010; 
	mem_reg_write = 1;
	wb_rd = 0;
	wb_reg_write = 0;
	#10;

	//Control Hazard with instruction flush
	reset = 0;
	de_rs1 = 0;
	de_rs2 = 0;
	ex_rs1 = 0;
	ex_rs2 = 0;
	ex_rd = 0;
	ex_pc_src = 1; // Control Hazard (Jump instruction)
	ex_result_src = 2'b00;
	mem_rd = 0;
	mem_reg_write = 0;
	wb_rd = 0;
	wb_reg_write = 0;
	#10;

	// Load-Use Data Hazard with forwarding (rs1)
	reset = 0;
	de_rs1 = 3'b010; // Load instruction
	de_rs2 = 0;
	ex_rs1 = 3'b010; // Load instruction
	ex_rs2 = 0;
	ex_rd = 0;
	ex_pc_src = 0;
	ex_result_src = 2'b00;
	mem_rd = 3'b010; 
	mem_reg_write = 1;
	wb_rd = 0;
	wb_reg_write = 0;
	#10;

	// Data Hazard with forwarding (rs1 and rs2)
	reset = 0;
	de_rs1 = 3'b010;
	de_rs2 = 3'b011;
	ex_rs1 = 3'b010;
	ex_rs2 = 3'b011;
	ex_rd = 0;
	ex_pc_src = 0;
	ex_result_src = 2'b00;
	mem_rd = 0;
	mem_reg_write = 0;
	wb_rd = 0;
	wb_reg_write = 0;
	#10;

	// Scenario 8: Data Hazard with forwarding and branch instruction
	reset = 0;
	de_rs1 = 3'b010;
	de_rs2 = 3'b011;
	ex_rs1 = 3'b010;
	ex_rs2 = 3'b011;
	ex_rd = 0;
	ex_pc_src = 0;
	ex_result_src = 2'b00;
	mem_rd = 0;
	mem_reg_write = 0;
	wb_rd = 0;
	wb_reg_write = 0;

 
end
endmodule