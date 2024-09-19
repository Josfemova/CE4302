//`define ENABLE_HPS
//`default_nettype none

module top (
    ///////// CLOCK2 /////////
    input              CLOCK2_50,
    ///////// CLOCK3 /////////
    input              CLOCK3_50,
    ///////// CLOCK4 /////////
    input              CLOCK4_50,
    ///////// CLOCK /////////
    input              CLOCK_50,

    ///////// GPIO /////////
    inout [35:0] GPIO_0,
    inout [35:0] GPIO_1,
    ///////// HEX0 /////////
    output      [6:0]  HEX0,
    ///////// HEX1 /////////
    output      [6:0]  HEX1,
    ///////// HEX2 /////////
    output      [6:0]  HEX2,
    ///////// HEX3 /////////
    output      [6:0]  HEX3,
    ///////// HEX4 /////////
    output      [6:0]  HEX4,
    ///////// HEX5 /////////
    output      [6:0]  HEX5,
    ///////// KEY /////////
    input       [3:0]  KEY,

    ///////// LEDR /////////
    output [7:0] LEDR,

    ///////// SW /////////
    input [9:0] SW
);

  // KEY 0 es clk manual 
  wire debug_mode;
  assign debug_mode = SW[1];

  wire manual_clk;
  assign manual_clk = KEY[0];

	wire [31:0] addr;

	assign LEDR[0] = addr[0];

  wire clk;
  assign clk = (debug_mode)?  CLOCK_50 : manual_clk;

  wire reset;
  assign reset = SW[0];
  
	assign LEDR[1] = clk;
	assign LEDR[4] = reset;

  seven_segment_driver seg0(.data_in(addr[3:0]),   .data_out(HEX0));
  seven_segment_driver seg1(.data_in(addr[7:4]),   .data_out(HEX1));
  seven_segment_driver seg2(.data_in(addr[11:8]),  .data_out(HEX2));
  seven_segment_driver seg3(.data_in(addr[15:12]), .data_out(HEX3));
  seven_segment_driver seg4(.data_in(addr[19:16]), .data_out(HEX4));
  seven_segment_driver seg5(.data_in(addr[23:20]), .data_out(HEX5));
  
  sisa_final dut(
		.clk_clk(clk),                //          clk.clk
		.instr_export_instr_if(addr),  // instr_export.instr_if
		//.instr_export_instr_de,  //             .instr_de
		//.instr_export_instr_ex,  //             .instr_ex
		//.instr_export_instr_mem, //             .instr_mem
		//.instr_export_instr_wb,  //             .instr_wb
		.reset_reset_n(reset),          //        reset.reset_n
		.control_reset_vector_addr(0),
		.uart_RXD               (GPIO_0[0]),               //         uart.RXD
		.uart_TXD               (GPIO_0[1])
	);


endmodule
