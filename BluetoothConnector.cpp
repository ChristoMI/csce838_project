#include "pkt.h";
#include <string>

class BluetoothConnector
{
    public uint48_t address;
    public unsigned long pktId;

    BluetoothConnector(uint48_t address)    //Constructor
    {
        this.address = address;
        this.pktId = 0;
    }
    
    void initConn(uint48_t pairer, uint48_t pairee)  
    {

    }

    pkt createPkt(uint48_t dest, string message)
    {
        pkt packet;
        pkt* ptr = &packet;
        ptr->id = pktId;
        ptr->sender = this.address;
        ptr->dest = dest;
        ptr->message = message;
        pktId++;
        return packet;
    }

    void sendPkt(pkt* ptr)
    {

    }

    pkt receivePkt()
    {

    }
}