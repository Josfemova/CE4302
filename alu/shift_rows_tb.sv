module shift_rows_tb;

    logic [127:0] state_in;
    logic [127:0] state_out;

    // Instancia de ShiftRows
    shift_rows uut (
        .state_in(state_in),
        .state_out(state_out)
    );

    initial begin

        state_in = 128'h0;  

        #10;
			
		  // Prueba 1
        state_in = 128'h0123456789abcdef0123456789abcdef;
        #10; 

        $display("Input:  %h", state_in);
        $display("Output: %h", state_out);

        // Prueba 2
        state_in = 128'habcdef0123456789abcdef0123456789;
        #10; 

        $display("Input:  %h", state_in);
        $display("Output: %h", state_out);

        $finish;
    end


endmodule
