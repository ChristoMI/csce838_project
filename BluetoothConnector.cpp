#include "./pkt.h";
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "bluetooth.h"
#include "rfcomm.h"
#include <string>

class BluetoothConnector
{
    public bdaddr_t address;
    public uint32_t pktId;
    private int status, socket, client;

    BluetoothConnector(bdaddr_t address)    //Constructor
    {
        this.address = address;
        this.pktId = 0;
        this.status = -1;
        this.socket = -1;
        this.client = -1;
    }
    
    void AcceptConn()  
    {
        struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
        char buf[1024] = { 0 };
        int bytes_read;
        socklen_t opt = sizeof(rem_addr);

        // allocate socket
        this.socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

        // bind socket to port 1 of the first available 
        // local bluetooth adapter
        loc_addr.rc_family = AF_BLUETOOTH;
        loc_addr.rc_bdaddr = *BDADDR_ANY;
        loc_addr.rc_channel = (uint8_t) 1;
        bind(this.socket, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

        // put socket into listening mode
        listen(this.socket, 1);

        // accept one connection
        this.client = accept(this.socket, (struct sockaddr *)&rem_addr, &opt);

        ba2str( &rem_addr.rc_bdaddr, buf );
        fprintf(stderr, "accepted connection from %s\n", buf);
        memset(buf, 0, sizeof(buf));
    }

    void InitConn(char dest[18]) 
    {
        struct sockaddr_rc addr = { 0 };

        // allocate a socket
        this.socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

        // set the connection parameters (who to connect to)
        addr.rc_family = AF_BLUETOOTH;
        addr.rc_channel = (uint8_t) 1;
        str2ba( dest, &addr.rc_bdaddr );

        // connect to server
        this.status = connect(this.socket, (struct sockaddr *)&addr, sizeof(addr));
    }

    pkt CreatePkt(string message)
    {
        pkt packet;
        pkt* ptr = &packet;

        ptr->id = this.pktId;
        ptr->message = message;

        this.pktId++;
        return packet;
    }

    void SendPkt(pkt* ptr)
    {
        // send a message
        if( this.status == 0 ) {
            this.status = write(this.socket, "hello!", 6);
        }
        if( this.status < 0 ) perror("error");
    }

    pkt ListenPkt(char[1024] buf,)
    {
        int bytes_read;
        // read data from the client
        bytes_read = read(this.client, buf, sizeof(buf));
        if( bytes_read > 0 ) {
            printf("received [%s]\n", buf);
        }
    }

    void CloseConn()
    {
        close(this.client);
        close(this.socket);
    }
}