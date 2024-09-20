IRAM_LOADABLE_CODE_ADDR = 0x250
RAM_BASE = 0x20000 # datos a encriptar
IRAM_ENC_HDR_STATUS = 0x120
IRAM_ENC_HDR_OP = 0x124  # 0 si es encriptar, otro valor de lo contrario*/
IRAM_ENC_HDR_BLOCKS =0x128 # Indica tamaño de datos en multiplos de 16 bytes 
IRAM_ENC_HDR_AES_KEY =0x12c # Ubicacion llave inicial 
OFFSET_CMD =0x100 
OFFSET_CMD_ARG0 =0x104 
OFFSET_CMD_ARG1 =0x108 
OFFSET_CMD_ARG2 =0x10c 
OFFSET_LAST_STATUS =0x110 # 0 si todo bien, 1 si hay error 
CMD_JMP_ADDR =1 #ARG 0 contiene la dirección a saltar */
CMD_PING =2 # ARG 0 contiene cuantos ciclos se espera, ARG1 cuiantos PINGS */

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
    socket.sendall(wr_cmd.format(addr, data).encode(()))
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
    write_mem(socket, OFFSET_CMD_ARG0, IRAM_LOADABLE_CODE_ADDR)
    write_mem(socket, OFFSET_CMD, CMD_JMP_ADDR)

def load_data(socket, data_txt_file, aes_key, op):
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

        if(i%4 != 0):
            error("Datos provistos requieren padding!")

        blocks = i/4
        print(f"Se escribieron {blocks} bloques a memoria")
        write_mem(socket,IRAM_ENC_HDR_AES_KEY, aes_key)
        write_mem(socket,IRAM_ENC_HDR_BLOCKS, blocks)
        write_mem(socket, IRAM_ENC_HDR_OP, op)

def extract_data(socket, txt_file, blocks):
    """
    Extrae de la ram (128 bits * blocks) datos, lo escribe de 32 bits en 32 bits
    """
    with open(txt_file) as out:
        words = blocks*4 
        for i in range(0, words):
            out.writeline(f"{read_mem(socket, RAM_BASE + (i<<2))}")
    status = read_mem(socket, IRAM_ENC_HDR_STATUS)
    print(f"Se obtuvo código de resultado: {status}")



s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

for i range (0, 40):
    print(f"val at {i*4}: {read_mem(s, i*4)}")

s.close()