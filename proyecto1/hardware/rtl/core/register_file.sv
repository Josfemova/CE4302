module register_file (
    input clk,
    input we3,
    input [4:0] a1,
    input [4:0] a2,
    input [4:0] a3,
    input [127:0] wd3,
    output [127:0] rd1,
    output [127:0] rd2
);

  reg [31:0] sreg[23:0];
  reg [127:0] vreg[7:0];

  // selección de registros.
  // Nótese que si se aplica una operación vectorial entre registro vectorial
  // y escalar, el registro escalar se cuadriplica, de forma que se abren 
  // posibles manipulaciones convenientes de esta característica. Solo llenar 
  // de 0's es otra opción, pero no se tendrían estas posibilidades. 
  assign rd1 = (a1 == 0) ? 0 : (( a1[4:3] == 2'b11 )? vreg[a1[2:0]] : {4{sreg[a1]}});
  assign rd2 = (a2 == 0) ? 0 : (( a2[4:3] == 2'b11 )? vreg[a2[2:0]] : {4{sreg[a2]}});

  always @(posedge clk) begin
    if (we3) begin 
      if (a3[4:3] == 2'b11) begin 
        vreg[a3[2:0]] <= wd3;
      end else begin 
        sreg[a3] <= wd3[31:0];
      end
    end
  end

endmodule
