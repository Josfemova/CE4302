module key_schedule_tb;

    reg  [127:0] K;
    reg  [31:0]  result;
    wire [127:0] round_key;

    // Instancia key schedule
    key_schedule uut (
        .K(K),
        .result(result),
        .round_key(round_key)
    );

    initial begin
	 
        // Prueba 1
        K = 128'h00112233445566778899aabbccddeeff;
        // Definir valor representativo del resultado de SubWord(RotWord(W3)) XOR Rcon
        result = 32'hdeadbeef;
		  
        $display("Key (K): 0x%h", K);
        $display("result: 0x%h", result);

        #10;

        $display("Round Key: 0x%h", round_key);

        $finish;
    end

endmodule
