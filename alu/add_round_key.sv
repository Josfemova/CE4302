module add_round_key (
    input  logic [127:0] state,       
    input  logic [127:0] round_key,   // Clave de ronda de 128 bits (16 bytes)
    output logic [127:0] new_state    
);

    always_comb begin
        new_state = state ^ round_key;  // Operación XOR en paralelo
    end

endmodule
