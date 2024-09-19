IRAM_ENC_ROUDKEY_STORE  = 0x130
IRAM_LOADABLE_CODE_ADDR = 0x250
RAM_BASE = 0x20000 # datos a encriptar
IRAM_ENC_HDR_STATUS = 0x120
IRAM_ENC_HDR_OP = 0x124  # 0 si es encriptar, otro valor de lo contrario*/
IRAM_ENC_HDR_BLOCKS =0x128 # Indica tamaño de datos en multiplos de 16 bytes 
IRAM_ENC_HDR_AES_KEY =0x12c # Ubicacion llave inicial 

def dump_mem(addr):

def write_mem(addr, txt_file):
