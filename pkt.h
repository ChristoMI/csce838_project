#include <string>
#ifndef PKT_H
#define PKT_H

typedef struct pkt
{
    unsigned long id;
    uint16_t sender;
    uint16_t dest;
    string message;
}pkt;

#endif