module load_store_unit_tb;
  reg clk;
  reg reset;
  reg vector_op;
  reg [31:0] base_addr;
  reg [127:0] in_writedata;
  reg [31:0] in_readdata;
  wire stall;
  wire [31:0] current_mem_addr;
  wire [127:0] out_readdata;
  wire [31:0] out_writedata;

  load_store_unit dut (
    .clk(clk),
    .reset(reset),
    .vector_op(vector_op),
    .base_addr(base_addr),
    .in_writedata(in_writedata),
    .in_readdata(in_readdata),
    .stall(stall),
    .current_mem_addr(current_mem_addr),
    .out_readdata(out_readdata),
    .out_writedata(out_writedata)
  );

  initial begin
    clk = 0;
    forever #5 clk = ~clk; // Genera un reloj de 10 unidades de tiempo
  end

  initial begin
    reset = 1;
    vector_op = 0;
    base_addr = 32'h0000_0000;
    in_writedata = 128'h0;
    in_readdata = 32'h0;
    #10;

    reset = 0; // Desactivar el reset
    #10;

    // Caso 1: Operacion escalar
    in_readdata = 32'hDEADBEEF;
    vector_op = 0; // No es una operación vectorial
    #10;

    // Caso 2: Operacion vectorial
    base_addr = 32'h0000_0010;
    in_writedata = 128'h12345678_12345678_12345678_12345678; // Datos a escribir
    vector_op = 1; // Activar operación vectorial
    #10;

    // Probar lectura durante operacion vectorial
    in_readdata = 32'hCAFEBABE; // Datos leídos
    #10;

    $finish;
  end

  initial begin
    $monitor("Time: %0t | Reset: %b | Vector Op: %b | Base Addr: %h | In Write Data: %h | In Read Data: %h | Stall: %b | Current Mem Addr: %h | Out Read Data: %h | Out Write Data: %h", 
             $time, reset, vector_op, base_addr, in_writedata, in_readdata, stall, current_mem_addr, out_readdata, out_writedata);
  end
endmodule
