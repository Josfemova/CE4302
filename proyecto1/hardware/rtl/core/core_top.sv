//`default_nettype none 

module core_top (
    input clk,
    input reset,

    input [31:0] reset_vector_addr,
    
    // instr memory master signals
    //input instr_memory_waitrequest,
    input  [31:0] instr_memory_readdata,
    output [31:0] instr_memory_addr,
    output instr_memory_read_en,
    output [3:0] instr_memory_byteenable,
    
    // data memory master signals
    //input data_memory_waitrequest, 
    input  [31:0] data_memory_readdata, 
    output [31:0] data_memory_addr,
    output [31:0] data_memory_writedata,
    output data_memory_write_en,
    output data_memory_read_en,
    output [3:0] data_memory_byteenable,

    // debug signals 
    output [31:0] instr_if,
    output [31:0] instr_de,
    output [31:0] instr_ex,
    output [31:0] instr_mem,
    output [31:0] instr_wb,

    // cpu_regs mem slave 1 (para cpu a sí mismo)
    // mismo clock del cpu
    input [6:0] s1_address,
    input s1_read,
    output reg [31:0] s1_readdata,
    input s1_write,
    input [31:0] s1_writedata,
    
    // cpu_regs mem slave 2 (para jtag)
    input [6:0] s2_address,
    input s2_read,
    output reg [31:0] s2_readdata,
    input s2_write,
    input [31:0] s2_writedata,
    input s_reset
);

  assign instr_memory_byteenable = 4'b1111;
  assign data_memory_byteenable = 4'b1111;


  // ======== IF ============ /
  wire if_stall;
  wire [31:0] if_instr_rd;  // de la memoria rom 
  wire [31:0] if_pc_next_instr_mem;  // a addr de rom

  // ======== DE ============ /
  wire de_clear;
  wire de_stall;
  wire [31:0] de_instr;  // a la memoria rom y a reg de decode
  wire [31:0] de_pc;
  wire [31:0] de_pc_plus4;
  wire [4:0] de_rs1;
  wire [4:0] de_rs2;

  // ======== EX ============ /
  wire ex_clear;
  wire ex_stall;
  wire [31:0] ex_instr;  
  wire ex_reg_write;
  wire ex_mem_write;
  wire ex_mem_read;
  wire ex_jump;
  wire ex_jump_cond;
  wire [2:0] ex_jump_cond_type;
  wire [3:0] ex_alu_control;
  wire ex_alu_src_op1;
  wire ex_alu_src_op2;
  wire ex_pc_target_src;
  wire [1:0] ex_result_src;
  wire ex_vector_op;
  wire [31:0] ex_pc;
  wire [31:0] ex_pc_plus_4;
  wire [31:0] ex_imm_ext;
  wire [127:0] ex_rd1;
  wire [127:0] ex_rd2;
  wire [4:0] ex_rs1;
  wire [4:0] ex_rs2;
  wire [4:0] ex_rd;
  wire ex_pc_src;
  wire [31:0] ex_pc_target;

  // ======== MEM =========== /
  wire mem_clear;
  wire mem_stall;
  wire [31:0] mem_instr;  
  wire mem_reg_write;
  wire mem_mem_write;
  wire mem_mem_read;
  wire [1:0] mem_result_src;
  wire mem_vector_op;
  wire [127:0] mem_alu_result;
  wire [127:0] mem_write_data;
  wire mem_stall_all;
  wire [31:0] mem_pc_plus_4;
  wire [127:0] mem_imm_ext;
  wire [4:0] mem_rd;

  // wire de la memoria de datos 
  wire [31:0] mem_read_result;

  // ======== WB ============ /
  wire wb_clear;
  wire wb_stall;
  wire [31:0] wb_instr;  
  wire wb_reg_write;
  wire wb_vector_op;
  wire [1:0] wb_result_src;
  wire [127:0] wb_alu_result;
  wire [127:0] wb_read_result;
  wire [31:0] wb_pc_plus_4;
  wire [127:0] wb_imm_ext;
  wire [4:0] wb_rd;
  wire [127:0] wb_result;


  // ========= Hazard unit ========/ 
  // salidas hazard unit
  wire [1:0] ex_op1_forward;
  wire [1:0] ex_op2_forward;
  wire stall_all;

  // ========= Debugging ========/ 
  wire clk_internal;
  reg [63:0] cycle;
  reg [31:0] manual_step_en = 32'b0;
  reg [31:0] manual_steps;
  reg [15:0][31:0] scratch_mem; // regs vacíos RW
  wire halt_cpu = manual_step_en[0] && (manual_steps==0);
  wire [4:0] s1_addr = s1_address[6:2];
  wire [4:0] s2_addr = s2_address[6:2];
  wire [31:0][31:0] csrs_ro = {
      scratch_mem,    // 0x40
      128'b0,         // 0x30 - 0x34 - 0x38 - 0x3c
      96'b0,          // 0x24 - 0x28 - 0x2c
      instr_wb,       // 0x20
      instr_mem,      // 0x1c
      instr_ex,       // 0x18
      instr_de,       // 0x14
      instr_if,       // 0x10
      manual_steps,   // 0xc
      manual_step_en, // 0x8
      cycle // 64 b   // 0x0 //0x4
      };

  assign clk_internal = halt_cpu ? 1'b1 : clk; // hay que forzarlo a 1 si se apaga
  /*always @(posedge clk) begin 
    halt_cpu <= manual_step_en[0] && (manual_steps==0);
  end*/ 

  assign instr_memory_addr = if_pc_next_instr_mem;
  assign instr_memory_read_en = ~if_stall;
  //! OJO EL HACK UUUUFFFF 
  // La cosa es que parece que tener un read enable no hace mucho para mantener 
  // la instrucción que está esperando en el puerto de read de las memorias que
  // se instancian en Qsys. Solución? Un latch interno al CPU
  // Si el ciclo anterior hubo un deassert de read_en, se escoge el valor del 
  // latch para sacar la instrucción que va al IF en vez de lo que se lee en el
  // puerto de memoria 
  reg [31:0] saved_instruction = 32'b0;
  reg [31:0] use_current_instr_mem_readdata;
  reg last_reset;
  reg last_halt;
  always @(posedge clk) begin 
    last_reset <= reset;
    if(reset) begin 
        last_halt<=0;
        saved_instruction<=0;
        use_current_instr_mem_readdata <= 0; // basura en read port
    end else begin 
        last_halt <= halt_cpu;
        // magia a ver si arreglo el halt
        use_current_instr_mem_readdata <= (instr_memory_read_en | last_reset) && (~halt_cpu);
        if((~last_halt) && (~last_reset)) begin 
            saved_instruction <= if_instr_rd;
        end else begin 
            saved_instruction <= saved_instruction;
        end
    end
  end
  assign if_instr_rd = (use_current_instr_mem_readdata)? instr_memory_readdata : saved_instruction;
  //assign instr_memory_read_en = 1'b1;
  assign mem_clear = reset;
  assign wb_clear = reset;

  // debug signals 
  assign instr_if = if_instr_rd;
  assign instr_de = de_instr;
  assign instr_ex = ex_instr;
  assign instr_mem = mem_instr;
  assign instr_wb = wb_instr;

  

  stage_instruction_fetch instf (
      .clk(clk_internal),
      .reset(reset),
      .reset_vector_addr(reset_vector_addr),
      .de_clear(de_clear),
      .if_stall(if_stall),
      .de_stall(de_stall),
      .ex_pc_src(ex_pc_src),
      .ex_pc_target(ex_pc_target),
      .if_instr_rd(if_instr_rd),
      .if_pc_next_instr_mem(if_pc_next_instr_mem),  // a addr de rom
      .de_instr(de_instr),
      .de_pc(de_pc),
      .de_pc_plus4(de_pc_plus4)
  );

  stage_decode de (
      .clk(clk_internal),
      .ex_clear(ex_clear),
      .ex_stall(ex_stall),
      .de_instr(de_instr),
      .de_pc(de_pc),
      .de_pc_plus4(de_pc_plus4),
      .wb_result(wb_result),
      .wb_reg_write(wb_reg_write),
      .wb_rd(wb_rd),
      .ex_instr(ex_instr),
      .ex_reg_write(ex_reg_write),
      .ex_mem_write(ex_mem_write),
      .ex_mem_read(ex_mem_read),
      .ex_jump(ex_jump),
      .ex_jump_cond(ex_jump_cond),
      .ex_jump_cond_type(ex_jump_cond_type),
      .ex_alu_control(ex_alu_control),
      .ex_alu_src_op1(ex_alu_src_op1),
      .ex_alu_src_op2(ex_alu_src_op2),
      .ex_pc_target_src(ex_pc_target_src),
      .ex_result_src(ex_result_src),
      .ex_vector_op(ex_vector_op),
      .ex_pc(ex_pc),
      .ex_pc_plus_4(ex_pc_plus_4),
      .ex_imm_ext(ex_imm_ext),
      .ex_rd1(ex_rd1),
      .ex_rd2(ex_rd2),
      .ex_rd(ex_rd),
      .ex_rs1(ex_rs1),
      .ex_rs2(ex_rs2),
      .de_rs1(de_rs1),
      .de_rs2(de_rs2)
  );

  stage_execute ex (
      .clk(clk_internal),
      .reset(reset),
      .mem_clear(mem_clear),
      .mem_stall(mem_stall),
      .ex_instr(ex_instr),
      .ex_reg_write(ex_reg_write),
      .ex_mem_write(ex_mem_write),
      .ex_mem_read(ex_mem_read),
      .ex_jump(ex_jump),
      .ex_jump_cond(ex_jump_cond),
      .ex_jump_cond_type(ex_jump_cond_type),
      .ex_alu_control(ex_alu_control),
      .ex_alu_src_op1(ex_alu_src_op1),
      .ex_alu_src_op2(ex_alu_src_op2),
      .ex_pc_target_src(ex_pc_target_src),
      .ex_result_src(ex_result_src),
      .ex_vector_op(ex_vector_op),
      .ex_pc(ex_pc),
      .ex_pc_plus_4(ex_pc_plus_4),
      .ex_imm_ext(ex_imm_ext),
      .ex_rd1(ex_rd1),
      .ex_rd2(ex_rd2),
      .ex_rd(ex_rd),
      .wb_result(wb_result),
      .ex_op1_forward(ex_op1_forward),
      .ex_op2_forward(ex_op2_forward),
      .mem_instr(mem_instr),
      .mem_reg_write(mem_reg_write),
      .mem_mem_write(mem_mem_write),
      .mem_mem_read(mem_mem_read),
      .mem_result_src(mem_result_src),
      .mem_vector_op(mem_vector_op),
      .mem_alu_result(mem_alu_result),
      .mem_write_data(mem_write_data),
      .mem_pc_plus_4(mem_pc_plus_4),
      .mem_imm_ext(mem_imm_ext),
      .mem_rd(mem_rd),
      .ex_pc_src(ex_pc_src),
      .ex_pc_target(ex_pc_target)
  );

  //assign data_memory_addr = mem_alu_result;
  //assign data_memory_writedata   = mem_write_data;
  assign data_memory_write_en   = mem_mem_write;
  assign data_memory_read_en   = mem_mem_read;
  assign mem_read_result  = data_memory_readdata;

  stage_memory mem (
      .clk(clk_internal),
      .reset(reset),
      .wb_clear(wb_clear),
      .wb_stall(wb_stall),
      .mem_instr(mem_instr),
      .mem_reg_write(mem_reg_write),
      //.mem_mem_write(mem_mem_write),
      .mem_result_src(mem_result_src),
      .mem_vector_op(mem_vector_op),
      .mem_alu_result(mem_alu_result),
      .mem_write_data(mem_write_data),
      .mem_pc_plus_4(mem_pc_plus_4),
      .mem_imm_ext(mem_imm_ext),
      .mem_rd(mem_rd),
      .mem_read_result(mem_read_result),
      .mem_data_memory_addr(data_memory_addr),
      .mem_data_memory_writedata(data_memory_writedata),
      .mem_stall_all(mem_stall_all),
      .wb_instr(wb_instr),
      .wb_reg_write(wb_reg_write),
      .wb_result_src(wb_result_src),
      .wb_vector_op(wb_vector_op),
      .wb_alu_result(wb_alu_result),
      .wb_read_result(wb_read_result),
      .wb_pc_plus_4(wb_pc_plus_4),
      .wb_imm_ext(wb_imm_ext),
      .wb_rd(wb_rd)
  );

  stage_writeback wb (
      .wb_result_src(wb_result_src),
      .wb_vector_op(wb_vector_op),
      .wb_alu_result(wb_alu_result),
      .wb_read_result(wb_read_result),
      .wb_pc_plus_4(wb_pc_plus_4),
      .wb_imm_ext(wb_imm_ext),
      .wb_result(wb_result)
  );

  hazard_unit hazard_u (
      .reset(reset),
      .de_rs1(de_rs1),
      .de_rs2(de_rs2),
      .ex_rs1(ex_rs1),
      .ex_rs2(ex_rs2),
      .ex_rd(ex_rd),
      .ex_pc_src(ex_pc_src),
      .ex_result_src(ex_result_src),
      .mem_rd(mem_rd),
      .mem_reg_write(mem_reg_write),
      .wb_rd(wb_rd),
      .wb_reg_write(wb_reg_write),
      .stall_all(stall_all), //! intencional que triggeree error
      .if_stall(if_stall),
      .de_stall(de_stall),
      .ex_stall(ex_stall),
      .mem_stall(mem_stall),
      .wb_stall(wb_stall),
      .de_flush(de_clear),
      .ex_flush(ex_clear),
      .ex_op1_forward(ex_op1_forward),
      .ex_op2_forward(ex_op2_forward)
  );

  assign stall_all = mem_stall_all;
  
  
  always @(posedge clk) begin 
      if (reset) begin 
          manual_step_en <= 0;
          manual_steps <= 0;
          cycle <= 0;
          s1_readdata <= 1'b0;
          s2_readdata <= 1'b0;
      end else begin
          cycle <= (halt_cpu) ? cycle : cycle+1; //aumentar contador de ciclos
          if(manual_step_en[0] && (manual_steps != 0)) begin 
              manual_steps <= manual_steps -1; //disminuir steps
          end
          if(s1_read) s1_readdata <= csrs_ro[s1_addr];
          if(s2_read) s2_readdata <= csrs_ro[s2_addr];
          // prioridad de escritura a s1
          if(s1_write) begin 
              case(s1_addr)
                  default:
                      if(s1_addr[4]) begin // scratch_mem
                          scratch_mem[s1_addr[3:0]] <= s1_writedata;
                      end 
              endcase
          end 
          if(s2_write)begin
              if (s1_addr != s2_addr) begin
                  case(s2_addr)
                      4'h2: manual_step_en <= s2_writedata;
                      4'h3: manual_steps <= s2_writedata;
                      default: begin 
                          if(s2_addr[4]) begin // scratch_mem
                              scratch_mem[s2_addr[3:0]] <= s2_writedata;
                          end 
                      end 
                  endcase
              end
          end  
      end
  end 



endmodule
