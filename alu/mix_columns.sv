module mix_columns (
    input logic [127:0] state_in,
    output logic [127:0] state_out
);

    // Variables temporales para cada columna de 32 bits
    logic [31:0] column [0:3];
    logic [31:0] new_column [0:3];

    // Funcion de multiplicacion en GF(2^8)
    function logic [7:0] gf_mult;
        input logic [7:0] a, b;
        logic [7:0] temp;
        begin
            temp = 0;
            for (int i = 0; i < 8; i++) begin
                if (b[i]) temp = temp ^ (a << i);
            end
            gf_mult = temp;
        end
    endfunction

    always_comb begin
        // Dividir el estado en columnas de 32 bits (4 bytes por columna) 
        column[0] = state_in[31:0];
        column[1] = state_in[63:32];
        column[2] = state_in[95:64];
        column[3] = state_in[127:96];

        // Aplicar MixColumns a cada columna 
        new_column[0] = {
            gf_mult(8'h02, column[0][7:0])   ^ gf_mult(8'h03, column[1][7:0])   ^ gf_mult(8'h01, column[2][7:0])   ^ gf_mult(8'h01, column[3][7:0]),
            gf_mult(8'h02, column[0][15:8])  ^ gf_mult(8'h03, column[1][15:8])  ^ gf_mult(8'h01, column[2][15:8])  ^ gf_mult(8'h01, column[3][15:8]),
            gf_mult(8'h02, column[0][23:16]) ^ gf_mult(8'h03, column[1][23:16]) ^ gf_mult(8'h01, column[2][23:16]) ^ gf_mult(8'h01, column[3][23:16]),
            gf_mult(8'h02, column[0][31:24]) ^ gf_mult(8'h03, column[1][31:24]) ^ gf_mult(8'h01, column[2][31:24]) ^ gf_mult(8'h01, column[3][31:24])
        };

        new_column[1] = {
            gf_mult(8'h01, column[0][7:0])   ^ gf_mult(8'h02, column[1][7:0])   ^ gf_mult(8'h03, column[2][7:0])   ^ gf_mult(8'h01, column[3][7:0]),
            gf_mult(8'h01, column[0][15:8])  ^ gf_mult(8'h02, column[1][15:8])  ^ gf_mult(8'h03, column[2][15:8])  ^ gf_mult(8'h01, column[3][15:8]),
            gf_mult(8'h01, column[0][23:16]) ^ gf_mult(8'h02, column[1][23:16]) ^ gf_mult(8'h03, column[2][23:16]) ^ gf_mult(8'h01, column[3][23:16]),
            gf_mult(8'h01, column[0][31:24]) ^ gf_mult(8'h02, column[1][31:24]) ^ gf_mult(8'h03, column[2][31:24]) ^ gf_mult(8'h01, column[3][31:24])
        };

        new_column[2] = {
            gf_mult(8'h01, column[0][7:0])   ^ gf_mult(8'h01, column[1][7:0])   ^ gf_mult(8'h02, column[2][7:0])   ^ gf_mult(8'h03, column[3][7:0]),
            gf_mult(8'h01, column[0][15:8])  ^ gf_mult(8'h01, column[1][15:8])  ^ gf_mult(8'h02, column[2][15:8])  ^ gf_mult(8'h03, column[3][15:8]),
            gf_mult(8'h01, column[0][23:16]) ^ gf_mult(8'h01, column[1][23:16]) ^ gf_mult(8'h02, column[2][23:16]) ^ gf_mult(8'h03, column[3][23:16]),
            gf_mult(8'h01, column[0][31:24]) ^ gf_mult(8'h01, column[1][31:24]) ^ gf_mult(8'h02, column[2][31:24]) ^ gf_mult(8'h03, column[3][31:24])
        };

        new_column[3] = {
            gf_mult(8'h03, column[0][7:0])   ^ gf_mult(8'h01, column[1][7:0])   ^ gf_mult(8'h01, column[2][7:0])   ^ gf_mult(8'h02, column[3][7:0]),
            gf_mult(8'h03, column[0][15:8])  ^ gf_mult(8'h01, column[1][15:8])  ^ gf_mult(8'h01, column[2][15:8])  ^ gf_mult(8'h02, column[3][15:8]),
            gf_mult(8'h03, column[0][23:16]) ^ gf_mult(8'h01, column[1][23:16]) ^ gf_mult(8'h01, column[2][23:16]) ^ gf_mult(8'h02, column[3][23:16]),
            gf_mult(8'h03, column[0][31:24]) ^ gf_mult(8'h01, column[1][31:24]) ^ gf_mult(8'h01, column[2][31:24]) ^ gf_mult(8'h02, column[3][31:24])
        };

        // Concatenar las nuevas columnas para formar el estado de 128 bits
        state_out = {new_column[3], new_column[2], new_column[1], new_column[0]};
    end

endmodule
