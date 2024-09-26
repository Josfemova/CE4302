module inv_shift_rows_tb;

    logic [127:0] state_in;
    logic [127:0] state_out;

    // Instancia de InvShiftRows
    inv_shift_rows uut (
        .state_in(state_in),
        .state_out(state_out)
    );

    initial begin

        state_in = 128'h0;  

        #10;
		  // Pruebas inversas de shift rows tb
		  // Prueba 1
        state_in = 128'h23456701cdef89ab6701234589abcdef;
        #10; 

        $display("Input:  %h", state_in);
        $display("Output: %h", state_out);

        // Prueba 2
        state_in = 128'hcdef01ab6789234501abcdef23456789;
        #10; 

        $display("Input:  %h", state_in);
        $display("Output: %h", state_out);

        $finish;
    end


endmodule
