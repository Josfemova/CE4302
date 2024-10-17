module inv_mix_columns_tb;

    logic [127:0] state_in;
    logic [127:0] state_out;

    // Instanciar InvMixColumns
    inv_mix_columns uut (
        .state_in(state_in),
        .state_out(state_out)
    );

    initial begin

        state_in = 128'h0; //Estado inicial de state

        #10;
		  
		  //Pruebas inversas de mix columns tb
        // Prueba 1
        state_in = 128'he174a60e37ea93a3fbb3323adba857aa;
        #10; 

        $display("Input:  %h", state_in);
        $display("Output: %h", state_out);

        // Preuba 2
        state_in = 128'h473794ed40d4e4a5a3703aa6acef322c;
;
        #10; 

        $display("Input:  %h", state_in);
        $display("Output: %h", state_out);

        $finish;
    end

endmodule
