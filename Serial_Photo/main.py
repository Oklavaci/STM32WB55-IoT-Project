# Set the working directory - very important
import os
path="C:\\Users\\suleyman\\Desktop\\SerialPhoto"
os.chdir(path)

import time
from tinydbgen import getdbval, updatedbval, setdbval
from UART2Text_numpy import UART_read
from Text2Image import TextToImage

while True:
    UART_read()
    TextToImage()
    # time.sleep(0.4)