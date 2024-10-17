module imm_extend_tb;

  logic [31:0] instr = 32'b0000_0000_0000_0000_0000_0010_0000_1000; // addi x1, x2
  logic [3:0] imm_src;
  logic [31:0] imm_out;

  imm_extend extend(
      .imm_in(instr[31:6]),   // Los bits relevantes de instr
      .imm_src(imm_src),
      .imm_out(imm_out)
  );

  // Monitor para imprimir valores
  initial begin
      $monitor("Time: %0t | instr: %b | imm_src: %b | imm_out: %b", 
                $time, instr, imm_src, imm_out);
  end

  // Asignacion secuencial de valores a imm_src
  initial begin
      imm_src = 4'b0000; 
      #10 imm_src = 4'b0001;
      #10 imm_src = 4'b0010;
      #10 imm_src = 4'b0011;
      #10 imm_src = 4'b0100;
      #10 imm_src = 4'b0101;
      #10 imm_src = 4'b0110;
      #10 imm_src = 4'b0111;
      #10 imm_src = 4'b1000;
      #10 imm_src = 4'b1001;

      $stop; 
  end

endmodule
