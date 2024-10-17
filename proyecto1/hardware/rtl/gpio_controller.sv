/*
Ni modo, los pios de quartus no tienen el mismo
timing del cpu tons estos son necesarios 
*/

module gpio_controller #(parameter WIDTH=32) (
    // señales de avalon
    input clk,
    input reset,
    input [31:0] address,
    input read,
    output [WIDTH-1:0] readdata,
    input write,
    input [WIDTH-1:0] writedata,

    // señales externas 
    inout [WIDTH-1:0] port 
);

reg [WIDTH-1:0] dir;
reg [WIDTH-1:0] port_readata;
reg [WIDTH-1:0] port_writedata;


  genvar i;
  generate
    for (i = 0; i < 32; i = i + 1) begin : gen_pio_ctrl
      assign port[i]   = (dir[i]) ? port_writedata[i] : 1'bz;
      assign port_readata[i] = (dir[i]) ? port_writedata[i] : GPIO0[i];
    end
  endgenerate

  always @(posedge clk) begin
    if(read) readdata <= port_readata;
    if (write) begin
      case (addr[3:0])
        4'h0: dir <= writedata;
        4'h4: port <= writedata;
      endcase
    end
  end

endmodule 