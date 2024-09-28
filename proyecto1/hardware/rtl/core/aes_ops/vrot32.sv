module vrot32 (
    input logic unsigned [127:0] state_in,
    input logic [127:0] bits_to_rotate,
    output logic [127:0] state_out
);
    // Variables temporales para cada fila de la matriz 4x4 
    int offset;
    int unsigned bits_to_rot;

    always_comb begin 
        for (int i=0; i<4; i++) begin 
            offset = i*32;
            bits_to_rot = bits_to_rotate[offset+:5]; // rotaciones de máximo 32 bits
            state_out[offset+:32] = 
                (state_in[offset+:32] >> bits_to_rot) | 
                (state_in[offset+:32] << (32-bits_to_rot))  ;
        end
    end
endmodule