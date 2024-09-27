`timescale 1 ps / 1 ps

module system_tb;

  logic clk;
  logic reset_n;
  wire [31:0] instr;

  initial begin
    reset_n <= 0;
    #20;
    reset_n <= 1;
  end

  always begin
    clk <= 1;
    #5;
    clk <= 0;
    #5;
  end
  

 sisa_final dut(
		.clk_clk(clk),                //          clk.clk
		.reset_reset_n(reset_n),           //        reset.reset_n
		//.instr_export_instr_if(instr),  // instr_export.instr_if
		//.instr_export_instr_de(instr),  //             .instr_de
		.instr_export_instr_ex(instr),  //             .instr_ex
		//.instr_export_instr_mem, //             .instr_mem
		//.instr_export_instr_wb(instr),  //             .instr_wb
    .control_reset_vector_addr(0)
    //.uart_RXD               (GPIO_0[0]),               //         uart.RXD
		//.uart_TXD               (GPIO_0[1])
	);

 
  always @(posedge clk) begin
    if (instr == 32'h000001) begin
      #100;
      $display("Simulation ended");
      $stop;
    end

  end



endmodule
