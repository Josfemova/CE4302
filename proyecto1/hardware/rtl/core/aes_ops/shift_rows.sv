module shift_rows (
    input logic [127:0] state_in,
    output logic [127:0] state_out
);
    // Variables temporales para cada fila de la matriz 4x4 
    logic [31:0] column [0:3];

    always @(*) begin
        // Dividir el estado en columnas de 32 bits (4 bytes por columna) 
        column[0] = state_in[31:0];
        column[1] = state_in[63:32];
        column[2] = state_in[95:64];
        column[3] = state_in[127:96];

        state_out[31:0] = { 
             column[3][31:24], column[2][23:16], column[1][15:8], column[0][7:0]
        };
        state_out[63:32] = {
            column[0][31:24], column[3][23:16], column[2][15:8], column[1][7:0]
        };
        state_out[95:64] = {
            column[1][31:24], column[0][23:16], column[3][15:8], column[2][7:0]
        };
        state_out[127:96] = {
            column[2][31:24], column[1][23:16], column[0][15:8], column[3][7:0]
        };
    end
endmodule