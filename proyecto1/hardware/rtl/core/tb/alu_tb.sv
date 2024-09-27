module alu_tb;

  parameter W = 32;

  reg [3:0] UC;  // Control de la ALU
  reg [3:0] Flags;

  logic clk = 0;
  logic reset;

  reg [W-1:0] A, B;  // Entradas de la ALU
  wire [W-1:0] Q;    // Salida de la ALU

  alu #(
      .WIDTH(W)
  ) DUT (
      .op1(A),
      .op2(B),
      .alu_control(UC),
      .flags(Flags),
      .result(Q)
  );

  // Generador de reloj
  always #2 clk = ~clk;

  // Monitor para imprimir los resultados
  initial begin
    $monitor("Time: %0t | A: %0d | B: %0d | UC: %0b | Q: %0d", $time, A, B, UC, Q);
  end

  // Bloque de prueba
  initial begin
    // Inicialización
    reset = 0;

    // Prueba 1: Suma
    A = 10;
    B = 5;
    UC = 4'b0000;  // Operación de suma
    #10;           // Espera 10 unidades de tiempo

    // Prueba 2: Resta
    A = 15;
    B = 3;
    UC = 4'b0001;  // Operación de resta
    #10;

    // Prueba 3: AND
    A = 8;
    B = 12;
    UC = 4'b0010;  // Operación AND
    #10;

    // Prueba 4: OR
    A = 6;
    B = 3;
    UC = 4'b0011;  // Operación OR
    #10;

    // Prueba 5: XOR
    A = 9;
    B = 5;
    UC = 4'b0100;  // Operación XOR
    #10;

    // Termina la simulación
    $stop;
  end

endmodule
