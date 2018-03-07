#ifndef SOCKETCANPORT_H
#define SOCKETCANPORT_H

#include <string>
#include <vector>
#include <iostream>
#include <string.h>

#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "PortBase.h"


using namespace std;

class SocketCanPort : public PortBase
{
public:
    SocketCanPort();
    SocketCanPort(string canPort);
    ~SocketCanPort();

    long SetFilter(uint32_t canId, uint32_t mask);
    long GetMsg(uint32_t & canId, uint8_t * data, uint8_t &size);
    long PutMsg(const uint32_t &canId, uint8_t * const data, const uint8_t size);
    long GetNMT(uint8_t *data, uint8_t &size);

private:

    long Init(string canPort);

    long nbytes;


    struct pollfd poll_setId[1];
    struct pollfd poll_setNMT[1];
    long timeoutPoll;

};

#endif // SOCKETCANPORT_H
