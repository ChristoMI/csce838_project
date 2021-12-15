import sys
import bluetooth
import time
import os

addr = "98:8D:46:C4:8D:D4"
#addr = "F0:42:1C:A9:BC:25"

port = 1

data = sys.argv[1]

#data = "hello"

start_time = time.time()

sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )

executable_path = "/home/car/ryan_brown_ws/src/bluetooth_com/src/ErrorLog"

try:
    print("Trying to connect...")
    sock.connect((addr, port))

    print("Connection established, sending the message: " + str(data))
    sock.send(data)

    end_time = time.time() - start_time
    print("Data sent successfully in " + str(round(end_time)) + " seconds...")


except bluetooth.btcommon.BluetoothError as e:
    print(e)
    split_e = str(e).split("]")

    code = split_e[0].split(" ")[1]
    description = split_e[1][1:len(split_e[1])]
    description = description.replace(" ", "\ ")

    print("Error code and description are retreived. Logging...")

    os.system(executable_path + " " + str(code) + " " + str(description))

    print("Logging completed...")

sock.close()