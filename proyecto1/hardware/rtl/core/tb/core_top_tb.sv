module core_top_tb;

    // Entradas
    reg clk;
    reg reset;
    reg [31:0] reset_vector_addr;
    reg [31:0] instr_memory_readdata;
    reg [31:0] data_memory_readdata;
    reg [6:0] s1_address;
    reg s1_read;
    reg s1_write;
    reg [31:0] s1_writedata;
    reg [6:0] s2_address;
    reg s2_read;
    reg s2_write;
    reg [31:0] s2_writedata;
    reg s_reset;

    // Salidas
    wire [31:0] instr_memory_addr;
    wire instr_memory_read_en;
    wire [3:0] instr_memory_byteenable;
    wire [31:0] data_memory_addr;
    wire [31:0] data_memory_writedata;
    wire data_memory_write_en;
    wire data_memory_read_en;
    wire [3:0] data_memory_byteenable;
    wire [31:0] instr_if;
    wire [31:0] instr_de;
    wire [31:0] instr_ex;
    wire [31:0] instr_mem;
    wire [31:0] instr_wb;
    wire [31:0] s1_readdata;
    wire [31:0] s2_readdata;

    core_top dut (
        .clk(clk),
        .reset(reset),
        .reset_vector_addr(reset_vector_addr),
        .instr_memory_readdata(instr_memory_readdata),
        .instr_memory_addr(instr_memory_addr),
        .instr_memory_read_en(instr_memory_read_en),
        .instr_memory_byteenable(instr_memory_byteenable),
        .data_memory_readdata(data_memory_readdata),
        .data_memory_addr(data_memory_addr),
        .data_memory_writedata(data_memory_writedata),
        .data_memory_write_en(data_memory_write_en),
        .data_memory_read_en(data_memory_read_en),
        .data_memory_byteenable(data_memory_byteenable),
        .instr_if(instr_if),
        .instr_de(instr_de),
        .instr_ex(instr_ex),
        .instr_mem(instr_mem),
        .instr_wb(instr_wb),
        .s1_address(s1_address),
        .s1_read(s1_read),
        .s1_readdata(s1_readdata),
        .s1_write(s1_write),
        .s1_writedata(s1_writedata),
        .s2_address(s2_address),
        .s2_read(s2_read),
        .s2_readdata(s2_readdata),
        .s2_write(s2_write),
        .s2_writedata(s2_writedata),
        .s_reset(s_reset)
    );

    // Generacion del reloj
    initial begin
        clk = 0;
        forever #5 clk = ~clk; // Cambia de estado cada 5 unidades de tiempo
    end

    initial begin
        // Configuración inicial
        reset = 1;
        reset_vector_addr = 32'h00000000;
        instr_memory_readdata = 32'hDEADBEEF; // Datos ejemplo para la memoria de instrucciones
        data_memory_readdata = 32'hCAFECAFE; // Datos ejemplo para la memoria de datos
        s1_address = 7'b0000000;
        s1_read = 0;
        s1_write = 0;
        s1_writedata = 32'h00000000;
        s2_address = 7'b0000001;
        s2_read = 0;
        s2_write = 0;
        s2_writedata = 32'h00000000;
        s_reset = 0;

        #10 reset = 0;

        // Ejemplo de operacion de lectura
        #10 s1_read = 1; // Leer de s1
        #10 s1_read = 0; // Dejar de leer
        #10 s2_write = 1; // Escribir en s2
        s2_writedata = 32'h12345678; // Dato a escribir
        #10 s2_write = 0; // Dejar de escribir

        #100 $finish;
    end

    // Monitoreo de señales
    initial begin
        $monitor("Time: %0t | Reset: %b | Instr Memory Addr: %h | Data Memory Addr: %h | Instr IF: %h | S1 Read Data: %h | S2 Read Data: %h",
                 $time, reset, instr_memory_addr, data_memory_addr, instr_if, s1_readdata, s2_readdata);
    end

endmodule
