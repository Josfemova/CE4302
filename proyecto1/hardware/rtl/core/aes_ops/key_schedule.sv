module key_schedule(
    input  [127:0] K,       // K = {w0, w1, w2, w3} 
    input  [31:0]  result,  // SubWord(RotWord(W3)) XOR Rcon
    output [127:0] round_key // Output
);

    wire [31:0] w0, w1, w2, w3;
    wire [31:0] w4, w5, w6, w7;

    // Dividir la clave K
    assign w0 = K[31:0];       
    assign w1 = K[63:32];
    assign w2 = K[95:64];
    assign w3 = K[127:96];     

    // XOR en cascada 
    assign w4 = w0 ^ result;
    assign w5 = w1 ^ w4;
    assign w6 = w2 ^ w5;
    assign w7 = w3 ^ w6;

    // Concatenar las palabras para formar el round key 
    assign round_key = {w7, w6, w5, w4};

endmodule
