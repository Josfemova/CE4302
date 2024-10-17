module readonly_pio #(parameter PORT_WIDTH=32, ADDR_WIDTH=32) (
    // señales de avalon
    input clk,
    input reset,
    input [ADDR_WIDTH-1:0] address,
    input read,
    output [PORT_WIDTH-1:0] readdata,

    // señales externas 
    input [PORT_WIDTH-1:0] port 
);

reg [PORT_WIDTH-1:0] port_readata;

  always @(posedge clk) begin
    if (reset) readdata<=0;
    else if(read) readdata <= port_readata;
  end

endmodule 