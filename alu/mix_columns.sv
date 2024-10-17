module mix_columns(
    input  logic [127:0] state_in,  
    output logic [127:0] state_out  
);

    // Funcion de multiplicacion en GF(2^8)
    function logic [7:0] GF_Mult;
        input logic [7:0] a, b;
        logic [7:0] result, temp;
        begin
            result = 8'b0;
            temp = a;
            for (int i = 0; i < 8; i++) begin
                if (b[0]) begin
                    result = result ^ temp;  // Sumar si el bit menos significativo de b es 1
                end
                if (temp[7]) begin
                    temp = (temp << 1) ^ 8'h1B;  // Reducir con el polinomio irreducible
                end else begin
                    temp = temp << 1;
                end
                b = b >> 1;  // Desplazar a la derecha
            end
            GF_Mult = result;
        end
    endfunction

    // variables temporales 
    logic [7:0] state [0:15];
    logic [7:0] temp [0:15];

    // Separar state_in en columnas
    always_comb begin
        for (int i = 0; i < 16; i++) begin
            state[i] = state_in[127 - i*8 -: 8];
        end

        // Mix Columns
        for (int j = 0; j < 4; j++) begin
            temp[j * 4 + 0] = GF_Mult(8'h02, state[j * 4 + 0]) ^ GF_Mult(8'h03, state[j * 4 + 1]) ^ state[j * 4 + 2] ^ state[j * 4 + 3];
            temp[j * 4 + 1] = state[j * 4 + 0] ^ GF_Mult(8'h02, state[j * 4 + 1]) ^ GF_Mult(8'h03, state[j * 4 + 2]) ^ state[j * 4 + 3];
            temp[j * 4 + 2] = state[j * 4 + 0] ^ state[j * 4 + 1] ^ GF_Mult(8'h02, state[j * 4 + 2]) ^ GF_Mult(8'h03, state[j * 4 + 3]);
            temp[j * 4 + 3] = GF_Mult(8'h03, state[j * 4 + 0]) ^ state[j * 4 + 1] ^ state[j * 4 + 2] ^ GF_Mult(8'h02, state[j * 4 + 3]);
        end

        // Reconstruccion de state_out 
        for (int i = 0; i < 16; i++) begin
            state_out[127 - i*8 -: 8] = temp[i];
        end
    end

endmodule
