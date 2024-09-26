IRAM_LOADABLE_CODE_ADDR = 0x250
CSR_CYCLEL =0x7000
CSR_CYCLEH =0x7004
CSR_MANUAL_STEP_EN =0x7008
CSR_MANUAL_STEPS =0x700c
CSR_INSTR_IF =0x7010
CSR_INSTR_DE =0x7014
CSR_INSTR_EX =0x7018
CSR_INSTR_MEM =0x701c
CSR_INSTR_WB =0x7020
#/*Espacio acá es reservado para counters no implementado aun, puro extra*/
CSR_SCRATCH_START =0x7040
CSR_SCRATCH_END =0x7080

RAM_BASE =0x80000 #// datos a encriptar

ENC_HDR_STATUS =0x2000
ENC_HDR_OP =0x2004 #/*0 si es encriptar, otro valor de lo contrario*/
ENC_HDR_BLOCKS =0x2008 #/* Indica tamaño de datos en multiplos de 16 bytes */
ENC_HDR_AES_KEY =0x2010 #/* Ubicacion llave inicial */
ENC_DATA_AES_ROUNDKEY_LAST =0x20a0 #/* Ubicación llave final */ 

SCRATCH_CMD =0x7040 
SCRATCH_CMD_ARG0 =0x7044 
SCRATCH_CMD_ARG1 =0x7048 
SCRATCH_CMD_ARG2 =0x704c 
SCRATCH_LAST_STATUS =0x7050 # 0 si todo bien, 1 si hay error 
CMD_JMP_ADDR =1 # ARG 0 contiene la dirección a saltar
CMD_PING =2 # ARG 0 contiene cuantos ciclos se espera, ARG1 cuiantos PINGS


import socket
from time import sleep

HOST = '127.0.0.1'    
PORT = 2540           
rd_cmd = "jtag_read 0x{:04X}\n"
wr_cmd = "jtag_write 0x{:08X} 0x{:08X}\n"

def read_mem(socket, addr):
    socket.sendall(rd_cmd.format(addr).encode())
    text=""
    while len(text)<10:
        text += socket.recv(10).decode()
    socket.recv(10)
    val= 0
    try:
        val = int(text, 16)
    except:
        error(f"Couldn't read addr {addr}\n")
    return val

def write_mem(socket, addr, data):
    socket.sendall(wr_cmd.format(addr, data).encode())
    pass

def load_code(socket, txt_file):
    """
    Carga y ejecuta el código a partir de un .txt que por línea contiene una
    instrucción en binario 
    """
    with open(txt_file) as src:
        i=0
        for line in src:
            # convertir desde binario
            val = int(line.strip('\n'), 2) 
            # escribir cada 32 bits
            write_mem(socket, IRAM_LOADABLE_CODE_ADDR + (i<<2), val)
            i+=1
    write_mem(socket, OFFSET_CMD_ARG0, IRAM_LOADABLE_CODE_ADDR)
    write_mem(socket, OFFSET_CMD, CMD_JMP_ADDR)

def load_data(socket, data_txt_file, key_txt_file, op):
    """
    Se espera un txt con cada linea siendo un dato de 32 bits
    """
    with open(data_txt_file) as data:
        i=0
        for line in data:
            # convertir desde binario
            val = int(line.strip('\n'), 2) 
            # escribir cada 32 bits
            write_mem(socket, RAM_BASE + (i<<2), val)
            i+=1

        if(i%4 != 0):
            error("Datos provistos requieren padding!")

        blocks = i/4
        print(f"Se escribieron {blocks} bloques a memoria")
        
        with open(key_txt_file) as key:
            j=0
            for line in key:
                # convertir desde hex
                val = int(line.strip('\n'), 16) 
                # escribir cada 32 bits
                write_mem(socket, ENC_HDR_AES_KEY + (j<<2), val)
                j+=1

        write_mem(socket, ENC_HDR_BLOCKS, blocks)
        write_mem(socket, ENC_HDR_OP, op)

def extract_data(socket, txt_file, blocks):
    """
    Extrae de la ram (128 bits * blocks) datos, lo escribe de 32 bits en 32 bits
    """
    with open(txt_file) as out:
        words = blocks*4 
        for i in range(0, words):
            out.writeline(f"{read_mem(socket, RAM_BASE + (i<<2))}")
    status = read_mem(socket, ENC_HDR_STATUS)
    print(f"Se obtuvo código de resultado: {status}")

def get_pipeline_state(socket):
    csrs = [
        CSR_INSTR_IF,
        CSR_INSTR_DE,
        CSR_INSTR_EX,
        CSR_INSTR_MEM,
        CSR_INSTR_WB,
    ]
    return [read_mem(socket, csr) for csr in csrs]


"""
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

for i in range(0, 40):
    print(f"val at {i*4:08X}: {read_mem(s, i*4):08X}")
"""
#s.close()