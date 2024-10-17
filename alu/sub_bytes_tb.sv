module sub_bytes_tb();

    logic [127:0] state_in;
    logic [127:0] state_out;

    // Instancia de SubBytes
    sub_bytes uut (
        .state_in(state_in),
        .state_out(state_out)
    );

    initial begin
	 
        state_in = 128'h3243f6a8885a308d313198a2e0370734; // Valor de entrada
        
        #10; 

        // Mostrar los resultados
        $display("state_in:  %h", state_in);
        $display("state_out: %h", state_out);

        $finish;
    end
endmodule
