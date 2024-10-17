//`default_nettype none

typedef enum logic[3:0]{
    SCALAR,
    VECTOR_ACTIVE,
    VECTOR_EXIT
} ldst_state;

module load_store_unit (
    input clk, 
    input reset,
    input vector_op,
    input [31:0] base_addr,
    input [127:0] in_writedata, 
    input [31:0] in_readdata, 
    output reg stall, // when asserted low, value is ready
    output reg [31:0] current_mem_addr,
    output reg [127:0] out_readdata, // se ve en wb, no mem
    output reg [31:0] out_writedata
);

parameter VEC_LEN = 4; //DWORDS

ldst_state state = SCALAR;
int rd_wait_cycles;
int wr_wait_cycles;
reg [95:0] vector_readdata;
reg [127:0] saved_readdata;
reg prev_inst_vector;
reg [127:0] out_vector;

/// resultado de lectura se adelanta a siguiente stage
/// el valor de read_data se debe preservar
always @(posedge clk) begin 
    if (reset || state == VECTOR_EXIT) begin
        state <= SCALAR;  
        wr_wait_cycles <=0;
        rd_wait_cycles <=0;
    end
    case (state)
        SCALAR: begin 
            rd_wait_cycles <= 0;
            if(vector_op) begin 
                saved_readdata <= out_readdata;
                state <= VECTOR_ACTIVE;
                prev_inst_vector <=1;
                wr_wait_cycles <=1;
            end else begin 
                state <= SCALAR;
                prev_inst_vector <= 0;
                wr_wait_cycles <=0;
            end 
        end
        VECTOR_ACTIVE: begin 
            if(rd_wait_cycles != (VEC_LEN-3)) begin 
                state <=VECTOR_ACTIVE;
            end 
            else begin 
                state <=VECTOR_EXIT;
            end 
            
            vector_readdata[32*rd_wait_cycles +:32] <= in_readdata; 
            rd_wait_cycles <= rd_wait_cycles +1;
            wr_wait_cycles <= wr_wait_cycles +1;
        end 
        VECTOR_EXIT: begin 
            vector_readdata[32*rd_wait_cycles +:32] <= in_readdata; 
            //wr_wait_cycles <=0;
            //wr_wait_cycles <= wr_wait_cycles +1;
            // resto del estado se actualiza en el check de reset
        end 
    endcase 
end 

always @(*) begin 
    out_writedata = in_writedata[32*wr_wait_cycles+:32];
    case(state)
        SCALAR: begin 
            stall = vector_op;
            current_mem_addr = base_addr;
            if(prev_inst_vector) begin
                out_readdata = {in_readdata, vector_readdata};
            end else begin 
                out_readdata = {96'b0, in_readdata};
            end
        end 
        VECTOR_ACTIVE: begin 
            stall = 1;
            current_mem_addr = base_addr + ({rd_wait_cycles+1,2'b00});
            out_readdata = saved_readdata;
        end 
        VECTOR_EXIT: begin
            stall = 0;
            current_mem_addr = base_addr + ({rd_wait_cycles+1,2'b00});
            out_readdata = saved_readdata;
        end 
    endcase
end 

endmodule