#include "boundingBox.h"
#ifndef PKT_H
#define PKT_H

typedef struct pkt
{
    uint8_t carID;
    uint16_t pktID;
    uint8_t sessionID;
    uint8_t objectClassID;
    uint32_t timestamp;
    boundingBox boundingBox;
}pkt;

#endif