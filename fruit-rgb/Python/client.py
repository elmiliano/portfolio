from serial import Serial
from time import sleep
import requests
import socket
import sys

#PORT DEFINITION
stm32_port = Serial('/dev/cu.usbmodem1103', 115200, timeout=1000)

#SERVER DEFINITION
host = "10.0.0.23"
server_port = 5001

#API URL
url = "http://10.0.0.23:3000/data"

#VARIABLES
count = 0
dataSent = False

#
while True:
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((host, server_port)) # connect to server
            print("connected!")

            while (count < 15): # for 15 rgb values
                if (stm32_port.in_waiting > 0): # wait for message from stm32
                    data = stm32_port.read_until()
                    data = data.decode("utf-8")

                    s.sendall(data) # send to server
                    print(data)

                    count += 1

            dataSent = True
            count = 0
        
        except:
            print(sys.exc_info())
            break

        finally:   
            if dataSent:  
                s.close() # close connection to server
                print("closed!")

                sleep(1)

                response = requests.get(url) # get grade from database
                RGBid = response.json()[0]['idSensorGrades']
                grade = response.json()[0]['char_level']

                stm32_port.write(grade.encode()) # send grade to stm32
                print("sent " + grade + " from ID " + str(RGBid))
                
                dataSent = False # reset flag