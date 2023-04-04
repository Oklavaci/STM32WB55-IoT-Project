from time import sleep
from tinydbgen import getdbval, updatedbval, setdbval


print("--------- UART2Text_numpy - START ---------")

from pickle import FALSE, TRUE


# Read from the serial port (COM)
import serial
import time
serialPort = serial.Serial(port = "COM16", baudrate=115200,
                        bytesize=8, timeout=1000, stopbits=serial.STOPBITS_ONE)



    # Determine the location where the files will be saved
import os
path="C:\\Users\\suleyman\\Desktop\\SerialPhoto"
os.chdir(path)

# numpy will be used for array managment - very useful !!!
import numpy as np

def UART_read():
# I defined start and stop bits
    start_bit = np.zeros((2), 'uint8')
    stop_bit = np.zeros((2), 'uint8')


    # Number of rows and columns of the image
    number_of_rows = 240
    number_of_cols = 320

    # each pixel requires 2 packages ->  frame_length = row * col * 2
    frame_length = number_of_rows*number_of_cols*2

    dataArray = np.zeros((frame_length + 2), 'uint8') # we have +2 due to stop bits


    # Reading operation is exexuted only once
    operation_flag = TRUE
    while operation_flag == TRUE:
        #time.sleep(0.1)


        # wait until the start bit comes
        while ((start_bit[0] != 0xBE) or (start_bit[1] != 0xBE)):
            start_bit[0] = start_bit[1]
            buffer = serialPort.read(1) 
            if(len(buffer)>0):
                start_bit[1] = ord(buffer)

        

        print("Header is FOUND")
        # save the image coming from the COM port
        frame_number = 0
        while(frame_number<frame_length+2):
            buffer = serialPort.read(1)
            if(len(buffer)>0):
                dataArray[frame_number] = ord(buffer)

            frame_number = frame_number + 1
        
        stop_bit[0] = dataArray[frame_length]
        stop_bit[1] = dataArray[frame_length+1]

        # Check the stop bits
        if((stop_bit[0] == 0xDE) and (stop_bit[0] == 0xDE)):
            operation_flag = FALSE    # If the stop bits are valid, then the operation is terminated
        else:
            start_bit[0] = 0
            start_bit[1] = 0
            frame_number = 0
            print("UART2Text_numpy - STOP BIT IS WRONG")  # Create a warning in the terminal
            

            # Create an additional warning text in the operation folder
            text_buffer = "Check the UART2Text.py. Stop bit is not equal to the expected value"
            with open('UART2Text-Error.txt', 'w') as f:
                f.write(text_buffer)

        

    np.savetxt('UART2Text-Result.txt', dataArray, fmt='%d', newline=' ')   #  Save the data into a text file


    print("---------- UART2Text_numpy - END ----------")

UART_read()