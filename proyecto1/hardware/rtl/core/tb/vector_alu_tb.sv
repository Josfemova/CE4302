module vector_alu_tb;
    reg [127:0] op1;
    reg [127:0] op2;
    reg [3:0] alu_control;
    wire [127:0] result;

    vector_alu #(.WIDTH(128)) uut (
        .op1(op1),
        .op2(op2),
        .alu_control(alu_control),
        .result(result)
    );

    initial begin
        // Monitorear cambios 
        $monitor("Time: %0t | op1: %h | op2: %h | alu_control: %b | result: %h", 
                 $time, op1, op2, alu_control, result);
    end

    initial begin
        // Test VALU_OP_XOR
        op1 = 128'hFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF;
        op2 = 128'h00000000000000000000000000000000;
        alu_control = 4'b0000; // VALU_OP_XOR
        #10;

        // Test VALU_OP_ROT
        op1 = 128'h0123456789ABCDEF0123456789ABCDEF;
        op2 = 128'h00000000000000000000000000000004; // Rotate by 4 bits
        alu_control = 4'b0001; // VALU_OP_ROT
        #10;

        // Test VALU_OP_AES_SUBBYTES
        op1 = 128'h01FFFEEDCBA9876543210FEDCBA98765; 
        op2 = 128'h00000000000000000000000000000000; 
        alu_control = 4'b1000; // VALU_OP_AES_SUBBYTES
        #10;

        // Test VALU_OP_AES_INV_SUBBYTES
        op1 = 128'h01FFFEEDCBA9876543210FEDCBA98765; 
        op2 = 128'h00000000000000000000000000000000; 
        alu_control = 4'b1001; // VALU_OP_AES_INV_SUBBYTES
        #10;

        $finish;
    end
endmodule
