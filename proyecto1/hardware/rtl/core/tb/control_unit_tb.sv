module control_unit_tb;
  reg [2:0] op;
  reg [2:0] func3;
  reg [10:0] func11;
  wire reg_write;
  wire mem_write;
  wire jump;
  wire jump_cond;
  wire [2:0] jump_cond_type;
  wire [3:0] alu_control;
  wire alu_src_op1;
  wire alu_src_op2;
  wire pc_target_src;
  wire [3:0] imm_src;
  wire [1:0] result_src;

 
  control_unit cu (
    .op(op),
    .func3(func3),
    .func11(func11),
    .reg_write(reg_write),
    .mem_write(mem_write),
    .jump(jump),
    .jump_cond(jump_cond),
    .jump_cond_type(jump_cond_type),
    .alu_control(alu_control),
    .alu_src_op1(alu_src_op1),
    .alu_src_op2(alu_src_op2),
    .pc_target_src(pc_target_src),
    .imm_src(imm_src),
    .result_src(result_src)
  );
  
    initial begin
    $monitor("Time: %0t | op: %b | func3: %b | func11: %b | reg_write: %b | mem_write: %b | jump: %b | jump_cond: %b | jump_cond_type: %b | alu_control: %b | alu_src_op1: %b | alu_src_op2: %b | pc_target_src: %b | imm_src: %b | result_src: %b",
              $time, op, func3, func11, reg_write, mem_write, jump, jump_cond, jump_cond_type, alu_control, alu_src_op1, alu_src_op2, pc_target_src, imm_src, result_src);
  end
  
  
  initial begin
    //OP_A type instruction (op = 3'b000)
    op = 3'b000;
    func3 = 3'b001;
    func11 = 11'b11011011010;
    #10;
    
    //OP_C type instruction (op = 3'b010)
    op = 3'b010;
    func3 = 3'b010;
    func11 = 11'b11011011010;
    #10;
    
    //OP_G type instruction (op = 3'b110)
    op = 3'b110;
    func3 = 3'b001;
    func11 = 11'b11011011010;
    #10;
    
	 //OP_D type instruction (op = 3'b011)
    op = 3'b011;
    func3 = 3'b010;
    func11 = 11'b11011011010;
    #10;
    
    // OP_F type instruction (op = 3'b100)
    op = 3'b100;
    func3 = 3'b000;
    func11 = 11'b11011011010;
    #10;
    

  
    
  end
endmodule
