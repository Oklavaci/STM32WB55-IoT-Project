print("--------- Text2Imagey - START ---------")

import os
path="C:\\Users\\suleyman\\Desktop\\SerialPhoto"
os.chdir(path)


# numpy is used for array managment - very useful
import numpy as np


# For converting the numpy array into image
from PIL import Image

#database
img_num = 0

# Number of rows and columns of the image
number_of_rows = 240
number_of_cols = 320


# each pixel requires 2 packages ->  frame_length = row * col * 2
frame_length = number_of_rows*number_of_cols*2





def TextToImage():

    # Read the text file
    text_path = "C:\\Users\\suleyman\\Desktop\\SerialPhoto\\UART2Text-Result.txt"
    dataArray = np.loadtxt(text_path, delimiter=' ', usecols=range(frame_length)).astype(int)

    # Define the RGB array
    IncRatio = 1
    rgbArray = np.zeros((number_of_rows*IncRatio,number_of_cols*IncRatio,3), 'uint8')


    row = 0
    col = 0
    while( row  < number_of_rows ):
        while( col < number_of_cols ):
            location = ( row * number_of_cols + col ) * 2

            RGB_Value = dataArray[location+1]*256 + dataArray[location]
            
            pixelR = RGB_Value & 0b11111
            pixelG = (RGB_Value >> 5) & 0b111111
            pixelB = (RGB_Value >> 11) & 0b11111

            rgbArray[row*IncRatio:(row+1)*IncRatio,col*IncRatio:(col+1)*IncRatio,0] = pixelB * 8        # red quarter square
            rgbArray[row*IncRatio:(row+1)*IncRatio,col*IncRatio:(col+1)*IncRatio,1] = pixelG * 4  
                # blue quarter square
            rgbArray[row*IncRatio:(row+1)*IncRatio,col*IncRatio:(col+1)*IncRatio,2] = pixelR * 8        # green quarter square

            col = col + 1

        row = row + 1
        col = 0
        

    from tinydbgen import getdbval, updatedbval, setdbval
    setdbval(0) # to eliminate give 0 to set db value set the intended value
    img = Image.fromarray(rgbArray.astype('uint8')).convert('RGB')
    img.save("%s%s%s" %('LastImage',getdbval(),'.png'))
    updatedbval()

    print("---------- Text2Image - END ----------")

TextToImage()