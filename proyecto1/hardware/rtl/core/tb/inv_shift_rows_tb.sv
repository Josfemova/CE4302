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
        state_in = 128'h8923cd6701ab45ef8923cd6701ab45ef;
        #10; 

        $display("Input:  %h", state_in);
        $display("Output: %h", state_out);

        // Prueba 2
        state_in = 128'h23cd6701ab45ef8923cd6701ab45ef89;
        #10; 

        $display("Input:  %h", state_in);
        $display("Output: %h", state_out);

        $finish;
    end


endmodule
