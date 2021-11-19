#include "./pkt.h";
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "bluetooth.h"
#include "rfcomm.h"
#include <string>

#define 
class BluetoothConnector
{
    public bdaddr_t address;
    public unsigned long pktId;

    public int status, sckt, client;

    BluetoothConnector(bdaddr_t address)    //Constructor
    {
        this.address = address;
        this.pktId = 0;
    }
    
    int[2] acceptConn(bdaddr_t* address)  
    {
        struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
        char buf[1024] = { 0 };
        int s, client, bytes_read;
        socklen_t opt = sizeof(rem_addr);

        // allocate socket
        s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

        // bind socket to port 1 of the first available 
        // local bluetooth adapter
        loc_addr.rc_family = AF_BLUETOOTH;
        loc_addr.rc_bdaddr = *BDADDR_ANY;
        loc_addr.rc_channel = (uint8_t) 1;
        bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

        // put socket into listening mode
        listen(s, 1);

        // accept one connection
        client = accept(s, (struct sockaddr *)&rem_addr, &opt);

        ba2str( &rem_addr.rc_bdaddr, buf );
        fprintf(stderr, "accepted connection from %s\n", buf);
        memset(buf, 0, sizeof(buf));

        int[2] values = [s, client];
        return values;
    }

    int[2] initConn(char dest[18]) 
    {
        struct sockaddr_rc addr = { 0 };
        int s, status;

        // allocate a socket
        s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

        // set the connection parameters (who to connect to)
        addr.rc_family = AF_BLUETOOTH;
        addr.rc_channel = (uint8_t) 1;
        str2ba( dest, &addr.rc_bdaddr );

        // connect to server
        status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

        int[2] identifiers = [s, status];
        return identifiers;
    }

    pkt createPkt(string message)
    {
        pkt packet;
        pkt* ptr = &packet;

        ptr->id = this.pktId;
        ptr->message = message;

        this.pktId++;
        return packet;
    }

    void sendPkt(pkt* ptr)
    {
        // send a message
        if( status == 0 ) {
            status = write(s, "hello!", 6);
        }
        if( status < 0 ) perror("error");
    }

    pkt listenPkt(int client, char[1024] buf,)
    {
        // read data from the client
        bytes_read = read(client, buf, sizeof(buf));
        if( bytes_read > 0 ) {
            printf("received [%s]\n", buf);
        }
    }

    void closeConn(int client, int s)
    {
        close(client);
        close(s);
    }
}