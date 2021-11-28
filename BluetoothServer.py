import socket

ADDRESS = ''
car1Address = ''
car2Address = ''
PORT = 54321

connections = []
host = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host.setblocking(0)
host.bind((ADDRESS, PORT))
host.listen(2)

def close_socket(connection):
    try:
        connection.shutdown(socket.SHUT_RDWR)
    except:
        pass
    try:
        connection.close()
    except:
        pass

def read():
    for i in reversed(range(len(connections))):
        try:
            data, sender = connections[i][0].recvfrom(1500)
            return data, sender
        except (BlockingIOError, socket.timeout, OSError):
            pass
        except (ConnectionResetError, ConnectionAbortedError):
            close_socket(connections[i][0])
            connections.pop(i)
    return b''  # return empty if no data found

def writeToFile(data, sender):
    with open('mainLogFile.txt', 'w') as f:
        f.write('\n'.join(data, sender))

    if sender == car1Address:
        with open('car1LogFile.txt', 'w') as f:
            f.write('\n'.join(data, sender))

    if sender == car2Address:
        with open('car2LogFile.txt', 'w') as f:
            f.write('\n'.join(data, sender))

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
    try:
        con, addr = host.accept()
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
close_socket(host)