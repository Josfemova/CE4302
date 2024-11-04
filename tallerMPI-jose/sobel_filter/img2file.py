import cv2
import argparse

ap = argparse.ArgumentParser(description='Convert an image to a column file of pixel value')
ap.add_argument("-i","--input", required=True, help="name of input image")
ap.add_argument("-o","--output", required=True, help="name of output txt file")

args = vars(ap.parse_args())

image = cv2.imread(args["input"])
grayscale = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

fileImage = open(args["output"] ,'w')

rows,cols = grayscale.shape

fileImage.write(f"{rows}\n")
fileImage.write(f"{cols}\n")

for i in range(rows):
    for j in range(cols):
        k = grayscale[i,j]
        temp = f"{k}\n"
        fileImage.write(temp)

fileImage.close()
