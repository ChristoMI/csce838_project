#include <string>
#ifndef PKT_H
#define PKT_H

typedef struct pkt
{
    unsigned long id;
    uint48_t sender;
    uint48_t dest;
    string message;
}pkt;

#endif