import cv2
import argparse
import numpy as np
from textwrap import wrap

ap = argparse.ArgumentParser()
ap.add_argument("-i","--input", required=True,help="name of input txt file")
ap.add_argument("-o","--output", required=True,help="name of output file")

args = vars(ap.parse_args())

fileImage = open(args["input"],'r')
rows = int(fileImage.readline().strip('\n'))
cols = int(fileImage.readline().strip('\n'))
#img = np.zeros((rows,cols,4), np.uint8)
bs = []


for i in range(rows):
    for j in range(cols):
        pixel = fileImage.readline().strip('\n')
        vals = wrap(pixel, 2)
        vals.reverse()
        for val in vals:
            bs.append(int(val, 16))
        #img[i,j] = np.array([k0, k1, k2, k3])

fileImage.close()
data = bytearray(bs)


from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes

key = get_random_bytes(16)
cipher = AES.new(0x0123456789ABCDEF0123456789ABCDEF.to_bytes(16, 'little'), AES.MODE_ECB)
ciphertext = cipher.decrypt(data)

k = memoryview(ciphertext).tolist()

outFile = open(args["output"] ,'w')
outFile.write(f"{rows}\n")
outFile.write(f"{cols}\n")

for i in range(0, rows*cols*4, 4):
    temp = f"{k[i+3]:02X}{k[i+2]:02X}{k[i+1]:02X}{k[i+0]:02X}\n"
    outFile.write(temp)

outFile.close()


#cv2.imshow("", img)
#cv2.waitKey(4000)
#cv2.imwrite(args["output"],img)
