module control_unit (
    input [2:0] op,  //! tipo de op
    input [2:0] func3,  //! subtipo de op
    input [10:0] func11,  //! prefijo de tipo de op (reservado, sin usar por ahora)

    output reg_write,  //! enable de write a regsitro
    output mem_write,  //! enable de write a memoria 
    output mem_read,  //! enable de read a memoria 
    output jump,  //! indica que se tiene una instr de salto 
    output jump_cond,  //! indica que se tiene una instr de salto condicionado
    output [2:0] jump_cond_type,  //! especifica tipo de salto condicional

    output logic [3:0] alu_control,  //! selecciona operacion de alu
    output alu_src_op1,  //! selecciona si op1 es rs1
    output alu_src_op2,  //! selecciona si op2 es rs2
    output pc_target_src,  //! selecciona si op2 es rs2

    //! [0] indica si es sin signo
    //! [1] indica si es upper 
    //! [2:3] indican el tipo de operación (entre las que usan inmediato)
    output logic [3:0] imm_src,
    output logic [1:0] result_src,  //! selector de resultado final

    // Soporte Vectorial. Podría tener una forma más elaborada para soportar 
    // otras instrucciones que no sean solo puramente vec<-vec con vec ? quizá, 
    // pero la decisión ingenieril acá es "ese caso de uso no se va a dar tons 
    // pa que" 
    output logic vector_op
);

  typedef enum bit [2:0] {
    OP_A = 3'b000,  // rd rs1 rs2, func3 y func11. ALU con regs si func11 = 0
    OP_B = 3'b001,  // rd rs1 imm_16 func3, ALU con inmediatos 
    OP_C = 3'b010,  // rs1, rs2 , imm_16, func3.  Solo STM (escribe memoria)
    OP_D = 3'b011,  // rd, imm_21 clir, cuir y jll
    OP_E = 3'b100,  // reservado
    OP_F = 3'b101,  // rd, rs1, imm_16. ldm y jlrl
    OP_G = 3'b110,  // rs1, rs2, imm_18:2. Jumps condicionales
    OP_H = 3'b111   // reservado
  } op_type;
  
  typedef enum bit [2:0] {
    STM  = 3'b000,
    STV  = 3'b100
  } func3_c;

  typedef enum bit [2:0] {
    CLIR = 3'b000,
    CUIR = 3'b001,
    JLL  = 3'b010
  } func3_d;

  typedef enum bit [2:0] {
    LDM  = 3'b000,
    LDV  = 3'b100,
    JLRL = 3'b010
  } func3_f;

  typedef enum bit [1:0] {
    RESULT_SRC_ALURES = 2'b00,
    RESULT_SRC_MEMRD = 2'b01,
    RESULT_SRC_PCPLUS4 = 2'b10,
    RESULT_SRC_IMM = 2'b11  // CLIR y CUIR
  } result_src_t;

  assign reg_write = ~(op[1:0] == 2'b10);  // aplica para todos excepto tipo c y tipo g
  assign mem_write = (op == OP_C);
  assign mem_read = (op == OP_F) && (func3 == LDM || func3 == LDV);
  assign jump = (((op == OP_D) && (func3 == JLL)) || ((op == OP_F) && (func3 == JLRL)));
  assign jump_cond = (op == OP_G);
  assign jump_cond_type = func3;  // solo importa para tipo g
  assign alu_src_op1 = (op != OP_D);  // solo queremos que sea uno cuando haya que forzar un 0
  assign alu_src_op2 = (op != OP_A) && (op != OP_G);  // aplica para tipo b,c,d e o f
  assign pc_target_src = (op == OP_F); // source de pc target es ALURES si es JLRL
  
  always @(*) begin
    vector_op = 0;
    case (op)
      OP_A: begin
        alu_control = {func11[0],func3};
        imm_src = 4'b0;  // no importa
        result_src = RESULT_SRC_ALURES;
      end
      OP_B: begin
        alu_control = {1'b0,func3};
        imm_src = ((func3 == 3'b101) || (func3 == 3'b110)) ? 4'b0001 : 4'b0000;  // se extiende en 0 para shifts
        result_src = RESULT_SRC_ALURES;
      end
      OP_C: begin  // solo STM y STV
        alu_control = 4'b0;  // suma dir a escribir 
        imm_src = 4'b100;  // usa inmediato con signo extendido
        result_src = RESULT_SRC_ALURES;  // no necesita result
        vector_op = (func3 == STV);
      end
      OP_D: begin  // CLIR, CUIR y JLL
        alu_control = 4'b0;  // suma 
        case (func3)
          CLIR: begin
            imm_src = 4'b1100;
            result_src = RESULT_SRC_ALURES;
          end
          CUIR: begin
            imm_src = 4'b1110;
            result_src = RESULT_SRC_ALURES;
          end
          default: begin
            imm_src = 4'b1100;
            result_src = RESULT_SRC_PCPLUS4;
          end
        endcase
      end
      OP_E: begin  // vectorial
        alu_control = {func11[0],func3};
        imm_src = 4'b0;  // no importa
        result_src = RESULT_SRC_ALURES;
      end
      OP_F: begin
        alu_control = 4'b0;  // suma dir de ldm o registro de jump + label
        imm_src = 4'b0000;  // inmediato con extensión de signo
        result_src = (func3 == LDM || func3 == LDV) ? RESULT_SRC_MEMRD : RESULT_SRC_ALURES;
        vector_op = (func3 == LDV);
      end
      OP_G: begin
        alu_control = 4'b1;  // resta para hacer comp
        imm_src = 4'b1000;  // inmediato con extensión de signo, [18:2] 
        result_src = RESULT_SRC_ALURES;  // no necesita result
      end
      /*OP_H: begin 
            
        end */
      default: begin
        alu_control = 4'b00;
        imm_src = 4'b000;
        result_src = RESULT_SRC_ALURES;
      end
    endcase
  end


endmodule
