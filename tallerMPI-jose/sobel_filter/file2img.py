import cv2
import argparse
import numpy as np
import textwrap

ap = argparse.ArgumentParser()
ap.add_argument("-i","--input", required=True,help="name of input txt file")
ap.add_argument("-o","--output", required=True,help="name of output file")

args = vars(ap.parse_args())

fileImage = open(args["input"],'r')
rows = int(fileImage.readline().strip('\n'))
cols = int(fileImage.readline().strip('\n'))
img = np.zeros((rows,cols,1), np.uint8)

for i in range(rows):
    for j in range(cols):
        pixel = fileImage.readline().strip('\n')
        pixel = int(pixel)
        img[i,j] = pixel

cv2.imwrite(args["output"],img)
