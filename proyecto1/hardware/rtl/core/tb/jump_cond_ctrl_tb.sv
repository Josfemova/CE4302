module jump_cond_ctrl_tb;
  reg jump_cond;
  reg [2:0] jump_cond_type;
  reg [3:0] alu_flags;
  wire jump_cond_true;

  
  jump_cond_ctrl jcc (
    .jump_cond(jump_cond),
    .jump_cond_type(jump_cond_type),
    .alu_flags(alu_flags),
    .jump_cond_true(jump_cond_true)
  );

  initial begin
  $monitor("Time: %0t | jump_cond: %b | jump_cond_type: %b | alu_flags: %b | jump_cond_true: %b", 
            $time, jump_cond, jump_cond_type, alu_flags, jump_cond_true);
  end

  
  initial begin
    // no jump
    jump_cond = 0;
    jump_cond_type = 3'b000; 
    alu_flags = 4'b0000; 
    #10;

    // condicion de jump igual, flags de la alu iguales
    jump_cond = 1;
    jump_cond_type = 3'b000; 
    alu_flags = 4'b0100;
    #10;

    // condicion de jump igual, flags de la alu diferentes
    jump_cond = 1;
    jump_cond_type = 3'b000; 
    alu_flags = 4'b1100; 
    #10;
	 // condicion de jump no igual, flags de la alu diferentes
	jump_cond = 1;
	jump_cond_type = 3'b001; 
	alu_flags = 4'b1100; 
	#10;
	

	// condicion de jump mayor que y flags de alu indican lo mismo 
	jump_cond = 1;
	jump_cond_type = 3'b010; 
	alu_flags = 4'b0100; 
	#10;

	// condicion de jump menor que y flags de alu indican lo mismo 
	jump_cond = 1;
	jump_cond_type = 3'b011; // COND_LT
	alu_flags = 4'b0010; // Less than (LT)
	#10;

	// condicion de jump mayor o igual que y flags de alu indican lo mismo 
	jump_cond = 1;
	jump_cond_type = 3'b100; 
	alu_flags = 4'b1001; 
	#10;

    
  end
endmodule
