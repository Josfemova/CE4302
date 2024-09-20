import argparse
import sys

ap = argparse.ArgumentParser(description='Convert .txt init file to .mif')
ap.add_argument("-i","--input", required=True, help="name of input txt")
ap.add_argument("-o","--output", required=True, help="name of output files (before extension)")

args = vars(ap.parse_args())
txtfile = open(args["input"],'r')
miffile = open(args["output"]+".mif",'w')
hexfile = open(args["output"]+".hex",'w')

lines = txtfile.readlines()
if '\n' in lines: lines.remove('\n')
depth = len(lines)*4

initial_mif_text = f"""
WIDTH=32;
DEPTH={depth};
ADDRESS_RADIX=HEX;
DATA_RADIX=HEX;

CONTENT BEGIN
"""
miffile.write(initial_mif_text)


def twos(val):
    return (1<<8) - val                       

def get_byte(n, pos):
    return (n >> (pos<<3)) & 0xff # pos*3 

addr = 0
for line in lines:
    line = line.strip('\n')
    val = int(line,2)
    miffile.write(f"\t{addr:<6X}: {val:X};\n")

    #hex file 
    checksum = 4 + get_byte(addr, 1) + get_byte(addr,0) + get_byte(val, 3) + get_byte(val, 2) + get_byte(val,1) + get_byte(val,0)
    checksum = twos(checksum & 0xff)
    text=f":04{addr:04X}00{val:08X}{checksum & 0xFF:02X}\n"
    hexfile.write(text)

    addr += 1

miffile.write("END;")
hexfile.write(":00000001FF")


txtfile.close()
miffile.close()