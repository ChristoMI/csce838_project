import bluetooth

ADDRESS = ''
car1Address = ''
car2Address = ''
PORT = 54321

connections = []
server_sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
server_sock.bind(("",PORT))
server_sock.listen(2)
waitingStatement = True

def close_socket(connection):
    try:
        connection.close()
    except:
        pass

def read():
    for i in reversed(range(len(connections))):
        try:
            data, sender = connections[i][0].recvfrom(1024)
            print("Received " + data)
            return data, sender
        except (BlockingIOError, server_sock.timeout, OSError):
            pass
        except (ConnectionResetError, ConnectionAbortedError):
            close_socket(connections[i][0])
            connections.pop(i)
    return b''  # return empty if no data found

def writeToFile(data, sender):
    data = data.split(';')
    ds1 = str(data[0])
    c1 =  str(data[1])
    ts1 = str(data[2])
    class1 = str(data[3])
    bb1 = str(data[4])
    with open('mainLogFile.txt', 'w') as f:
            f.write('\n session_id: ' + ds1)
            f.write('\n car_id: ' + c1)
            f.write('\n timestamp: ' + ts1)
            f.write('\n object_class_id: ' + class1)
            f.write('\n bounding_box: ' + bb1)

    if sender == car1Address:
        with open('car1LogFile.txt', 'w') as f:
            f.write('\n session_id: ' + ds1)
            f.write('\n car_id: ' + c1)
            f.write('\n timestamp: ' + ts1)
            f.write('\n object_class_id: ' + class1)
            f.write('\n bounding_box: ' + bb1)

    if sender == car2Address:
        with open('car2LogFile.txt', 'w') as f:
            f.write('\n session_id: ' + ds1)
            f.write('\n car_id: ' + c1)
            f.write('\n timestamp: ' + ts1)
            f.write('\n object_class_id: ' + class1)
            f.write('\n bounding_box: ' + bb1)

    # for i in reversed(range(len(connections))):
    #     try:
    #         connections[i][0].sendto(data, connections[i][1])
    #     except (BlockingIOError, socket.timeout, OSError):
    #         pass
    #     except (ConnectionResetError, ConnectionAbortedError):
    #         close_socket(connections[i][0])
    #         connections.pop(i)

# Run the main loop
while True:
    if waitingStatement:
        print("Waiting for connection...")
        waitingStatement = False

    try:
        con, addr = server_sock.accept()
        connections.append((con, addr))
    except BlockingIOError:
        pass

    data, sender = read()
    if data != b'':
        print(data, sender)
        writeToFile(data, sender)
        if data == b"exit":
            break

# Close the sockets
for i in reversed(range(len(connections))):
    close_socket(connections[i][0])
    connections.pop(i)
close_socket(server_sock)