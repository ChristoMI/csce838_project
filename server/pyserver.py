'''
Created on Nov 27, 2021

@author: crescendo
'''

import bluetooth
import math

import datetime

connection_adresses = []

connection_data_map = {}

connection_datafile_map = {}

path = "/home/crescendo/Workspace/csce_838/data"

keywords = [
        "car_id",
        "timestamp",
        "object_class_id",
        "bounding_box"
    ]

bb_keywords = [
        "minX",
        "maxX",
        "minY",
        "maxY"
    ]

def compose_string_dict(data):
    split = data.split(";")
        
    result = "new_object\n{\n"
        
    for i in range(len(keywords) - 1):
        result += "\t\"" + keywords[i] + "\" : \"" + split[i] + "\",\n"
    
    result += "\t\"bounding_box\" : \n\t\t{\n"
    
    for i in range(len(bb_keywords)):
        result += "\t\t\"" + bb_keywords[i] + "\" : \"" + split[i + len(keywords) - 1] + "\",\n"

            
    return result + "\t\t}\n}\n"

while(True):
    server_sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
    
    port = 1
    server_sock.bind(("",port))
    server_sock.listen(1)
    
    
    print("Waiting for connection....")
    client_sock,address = server_sock.accept()
    
    #address = ["address:adress", "1"]
    
    mac_string = address[0].replace(":", ".")
    
    string_address = str(mac_string) + "_" + str(address[1])
    
    if string_address not in connection_adresses:
        connection_adresses.append(string_address)
    
    print("Accepted connection from " + string_address)
    
    #received_data = client_sock.recv(1024)
    
    data = int(client_sock.recv(1024).decode("utf-8"))
    
    if string_address not in connection_data_map.keys():
        connection_data_map[string_address] = []
    
    data_copy = data
    data_string = ""
    
    class_id = data_copy % 10;
    data_copy = math.floor(data_copy / 10);
    data_string += (str(class_id) + ";")
    
    minX = data_copy % 1000;
    data_copy = math.floor(data_copy / 1000);
    data_string += (str(minX) + ";")
    
    maxX = data_copy % 1000;
    data_copy = math.floor(data_copy / 1000);
    data_string += (str(maxX) + ";")
    
    minY = data_copy % 1000;
    data_copy = math.floor(data_copy / 1000);
    data_string += (str(minY) + ";")
    
    maxY = data_copy % 1000;
    data_copy = math.floor(data_copy / 1000);
    data_string += (str(maxY))
    
    connection_data_map[string_address].append(data_string)
    
    #data = "DATA1;DATA2;DATA3;DATA4;DATA5"
    print("Received \'" + data_string + '\'\n')


    data_to_process = string_address + ";" + str(datetime.datetime.now()) + ";" + data_string

    print(data_to_process)

    current_file = open(path + "/" + string_address + ".txt", "a")  # append mode
    current_file.write(compose_string_dict(data_to_process))
    current_file.close()

    client_sock.close()
    server_sock.close()
    

    