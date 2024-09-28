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
img = np.zeros((rows,cols,4), np.uint8)

for i in range(rows):
    for j in range(cols):
        pixel = fileImage.readline().strip('\n')
        vals = wrap(pixel, 2)
        k3 = int(vals[0], 16)
        k2 = int(vals[1], 16)
        k1 = int(vals[2], 16)
        k0 = int(vals[3], 16)
        img[i,j] = np.array([k0, k1, k2, k3])

cv2.imshow("", img)
cv2.waitKey(4000)
cv2.imwrite(args["output"],img)
