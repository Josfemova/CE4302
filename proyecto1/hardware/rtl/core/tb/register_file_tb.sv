module register_file_tb;
  reg clk;
  reg we3;
  reg [4:0] a1;
  reg [4:0] a2;
  reg [4:0] a3;
  reg [127:0] wd3;
  wire [127:0] rd1;
  wire [127:0] rd2;

  register_file dut (
    .clk(clk),
    .we3(we3),
    .a1(a1),
    .a2(a2),
    .a3(a3),
    .wd3(wd3),
    .rd1(rd1),
    .rd2(rd2)
  );

  initial begin
    clk = 0;
    forever #5 clk = ~clk;
  end

  initial begin
    $monitor("Time: %0t | we3: %b | a1: %d | a2: %d | a3: %d | wd3: %h | rd1: %h | rd2: %h", 
             $time, we3, a1, a2, a3, wd3, rd1, rd2);

    we3 = 1;
    a1 = 10;
    a2 = 15;
    a3 = 5;
    wd3 = 128'h0ABCDE123_0ABCDE123_0ABCDE123_0ABCDE123;
    #10;
    we3 = 0;
    #10;

    we3 = 0;
    a1 = 0;
    a2 = 31;
    a3 = 0;
    wd3 = 128'h0;
    #10;

    we3 = 1;
    a1 = 14;
    a2 = 21;
    a3 = 7;
    wd3 = 128'h12345678_12345678_12345678_12345678;
    #10;
    we3 = 0;
    #10;

    we3 = 1;
    a1 = 2;
    a2 = 3;
    a3 = 1;
    wd3 = 128'hABCDEFFF_ABCD1234_ABCD5678_ABCD9ABC;
    #10;
    we3 = 0;
    #10;

    $finish;
  end
endmodule
