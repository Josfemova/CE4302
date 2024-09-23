//Modulo top para evitar problemas con entradas de 128 bits

module top_module (
    input logic clk,
    output logic [127:0] output_state  // Conectar la salida del módulo add_round_key
);

    // Señales internas 
    logic [127:0] input_state;   
    logic [127:0] input_round_key; 
    logic [127:0] internal_output_state;   

    // Instanciar add_round_key
    add_round_key u_add_round_key (
        .state(input_state),        
        .round_key(input_round_key), 
        .new_state(internal_output_state)    
    );


endmodule
