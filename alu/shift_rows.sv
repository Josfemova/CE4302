module shift_rows (
    input logic [127:0] state_in,
    output logic [127:0] state_out
);
    // Variables temporales para cada fila de la matriz 4x4 
    logic [31:0] row [0:3];

    always_comb begin
        // Dividir el estado en filas de 32 bits (4 bytes por fila)
        row[0] = state_in[31:0];    
        row[1] = state_in[63:32];   
        row[2] = state_in[95:64];   
        row[3] = state_in[127:96];  

        // Aplicar el desplazamiento circular a cada fila
        state_out[31:0]    = row[0];                          // Primera fila (sin cambios)
        state_out[63:32]   = {row[1][7:0], row[1][31:8]};     // Segunda fila (rotar 1 byte a la izquierda)
        state_out[95:64]   = {row[2][15:0], row[2][31:16]};   // Tercera fila (rotar 2 bytes a la izquierda)
        state_out[127:96]  = {row[3][23:0], row[3][31:24]};   // Cuarta fila (rotar 3 bytes a la izquierda)
    end
endmodule
