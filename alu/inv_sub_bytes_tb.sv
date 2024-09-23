module inv_sub_bytes_tb;
    logic [127:0] state_in;
    logic [127:0] state_out;

    // Instanciación de inv_sub_bytes
    inv_sub_bytes uut (
        .state_in(state_in),
        .state_out(state_out)
    );

    initial begin
        // Prueba con el output de subbytes tb
        state_in = 128'h231a42c2c4be045dc7c7463ae19ac518;
		  //Salida esperada: 3243f6a8885a308d313198a2e0370734

        #10;  
        $display("Input:       %h", state_in);
        $display("SubBytes Inv: %h", state_out);
		  
        $finish;
    end
endmodule
