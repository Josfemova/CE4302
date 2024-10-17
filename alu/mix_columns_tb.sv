module mix_columns_tb;

    logic [127:0] state_in;
    logic [127:0] state_out;

    // Instanciar MixColumns
    mix_columns uut (
        .state_in(state_in),
        .state_out(state_out)
    );

    initial begin

        state_in = 128'h0; //Estado inicial de state

        #10;

        // Prueba 1
        state_in = 128'h19f624f6f4e3916b14ce5dc7a9d722d2;
        #10; 

        $display("Input:  %h", state_in);
        $display("Output: %h", state_out);

        // Preuba 2
        state_in = 128'h876e46a6f24ce78c4d904ad8e7ecc395;
;
        #10; 

        $display("Input:  %h", state_in);
        $display("Output: %h", state_out);

        $finish;
    end

endmodule
