module vrto32_tb;

    logic [127:0] state_in;
    logic [127:0] state_out;
    logic [127:0] bits_to_rotate;

    // Instancia de ShiftRows
    vrot32 uut (
        .state_in(state_in),
        .state_out(state_out),
        .bits_to_rotate(bits_to_rotate)
    );

    initial begin

        state_in = 128'h0;  

        #10;
			
		  // Prueba 1
        state_in = 128'haabbccdd_eeff0011_2233bb00_aabbccdd;
        bits_to_rotate = 128'h00000008_00000010_00000018_00000020;
        #10; 

        $display("Input:  %h", state_in);
        $display("rot:    %h", bits_to_rotate);
        $display("Output: %h", state_out);

        // Prueba 2
        state_in = 128'h44000011_33000022_55000077_11000044;
        bits_to_rotate = 128'h00000028_00000030_00000038_00000040;
        #10; 

        $display("Input:  %h", state_in);
        $display("rot:    %h", bits_to_rotate);
        $display("Output: %h", state_out);

        $finish;
    end


endmodule
