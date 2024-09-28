import cv2
import argparse

ap = argparse.ArgumentParser(description='Convert an image to a column file of pixel value')
ap.add_argument("-i","--input", required=True, help="name of input image")
ap.add_argument("-o","--output", required=True, help="name of output txt file")

args = vars(ap.parse_args())

colorIm = cv2.imread(args["input"], cv2.IMREAD_UNCHANGED)
colorIm = cv2.resize(colorIm, (256, 256))

fileImage = open(args["output"] ,'w')

rows,cols, depth = colorIm.shape
if(depth != 4):
    print(depth)
    exit("profundidad no soportada")

fileImage.write(f"{rows}\n")
fileImage.write(f"{cols}\n")

for i in range(rows):
    for j in range(cols):
        k = colorIm[i,j]
        temp = f"{k[3]:02X}{k[2]:02X}{k[1]:02X}{k[0]:02X}\n"
        fileImage.write(temp)

fileImage.close()