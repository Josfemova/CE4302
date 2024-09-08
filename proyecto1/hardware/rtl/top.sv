//`define ENABLE_HPS
//`default_nettype none

module top (

    ///////// ADC /////////
    output ADC_CONVST,
    output ADC_SCK,
    output ADC_SDI,
    input  ADC_SDO,

    ///////// ARDUINO /////////
    inout [15:0] ARDUINO_IO,
    inout        ARDUINO_RESET_N,

    ///////// FPGA /////////
    input FPGA_CLK1_50,
    input FPGA_CLK2_50,
    input FPGA_CLK3_50,

    ///////// GPIO /////////
    inout [35:0] GPIO_0,
    inout [35:0] GPIO_1,

    ///////// HDMI /////////
    inout         HDMI_I2C_SCL,
    inout         HDMI_I2C_SDA,
    inout         HDMI_I2S,
    inout         HDMI_LRCLK,
    inout         HDMI_MCLK,
    inout         HDMI_SCLK,
    output        HDMI_TX_CLK,
    output [23:0] HDMI_TX_D,
    output        HDMI_TX_DE,
    output        HDMI_TX_HS,
    input         HDMI_TX_INT,
    output        HDMI_TX_VS,

`ifdef ENABLE_HPS
    ///////// HPS /////////
    inout         HPS_CONV_USB_N,
    output [14:0] HPS_DDR3_ADDR,
    output [ 2:0] HPS_DDR3_BA,
    output        HPS_DDR3_CAS_N,
    output        HPS_DDR3_CKE,
    output        HPS_DDR3_CK_N,
    output        HPS_DDR3_CK_P,
    output        HPS_DDR3_CS_N,
    output [ 3:0] HPS_DDR3_DM,
    inout  [31:0] HPS_DDR3_DQ,
    inout  [ 3:0] HPS_DDR3_DQS_N,
    inout  [ 3:0] HPS_DDR3_DQS_P,
    output        HPS_DDR3_ODT,
    output        HPS_DDR3_RAS_N,
    output        HPS_DDR3_RESET_N,
    input         HPS_DDR3_RZQ,
    output        HPS_DDR3_WE_N,
    output        HPS_ENET_GTX_CLK,
    inout         HPS_ENET_INT_N,
    output        HPS_ENET_MDC,
    inout         HPS_ENET_MDIO,
    input         HPS_ENET_RX_CLK,
    input  [ 3:0] HPS_ENET_RX_DATA,
    input         HPS_ENET_RX_DV,
    output [ 3:0] HPS_ENET_TX_DATA,
    output        HPS_ENET_TX_EN,
    inout         HPS_GSENSOR_INT,
    inout         HPS_I2C0_SCLK,
    inout         HPS_I2C0_SDAT,
    inout         HPS_I2C1_SCLK,
    inout         HPS_I2C1_SDAT,
    inout         HPS_KEY,
    inout         HPS_LED,
    inout         HPS_LTC_GPIO,
    output        HPS_SD_CLK,
    inout         HPS_SD_CMD,
    inout  [ 3:0] HPS_SD_DATA,
    output        HPS_SPIM_CLK,
    input         HPS_SPIM_MISO,
    output        HPS_SPIM_MOSI,
    inout         HPS_SPIM_SS,
    input         HPS_UART_RX,
    output        HPS_UART_TX,
    input         HPS_USB_CLKOUT,
    inout  [ 7:0] HPS_USB_DATA,
    input         HPS_USB_DIR,
    input         HPS_USB_NXT,
    output        HPS_USB_STP,
`endif  /*ENABLE_HPS*/

    ///////// KEY /////////
    input [1:0] KEY,

    ///////// LED /////////
    output [7:0] LED,

    ///////// SW /////////
    input [3:0] SW
);

  // KEY 0 es clk manual 
  wire debug_mode;
  assign debug_mode = SW[1];

  wire manual_clk;
  assign manual_clk = KEY[0];

	wire [31:0] addr;

	assign LED[0] = addr[0];

  wire clk;
  assign clk = (debug_mode)?  FPGA_CLK1_50 : manual_clk;

  wire reset;
  assign reset = SW[0];
  sisa_test dut(
		.clk_clk(clk),                //          clk.clk
		.instr_export_instr_if(addr),  // instr_export.instr_if
		//.instr_export_instr_de,  //             .instr_de
		//.instr_export_instr_ex,  //             .instr_ex
		//.instr_export_instr_mem, //             .instr_mem
		//.instr_export_instr_wb,  //             .instr_wb
		.reset_reset_n(reset)           //        reset.reset_n
	);


endmodule
