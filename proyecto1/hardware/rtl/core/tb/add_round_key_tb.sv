module add_round_key_tb;

    logic [127:0] state;
    logic [127:0] round_key;
    logic [127:0] new_state;

    // Instanciar AddRoundKey
    add_round_key uut (
        .state(state),
        .round_key(round_key),
        .new_state(new_state)
    );

    initial begin

        state     = 128'h00112233445566778899AABBCCDDEEFF;
        round_key = 128'h0F0E0D0C0B0A09080706050403020100;

        #1;
		  
        $display("State:        %h", state);
        $display("Round Key:    %h", round_key);
        $display("New State:    %h", new_state);

        $finish;
    end

endmodule
