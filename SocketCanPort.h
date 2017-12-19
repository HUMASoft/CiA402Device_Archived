#ifndef SOCKETCANPORT_H
#define SOCKETCANPORT_H

#include <string>
#include <vector>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
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


    long Init(string canPort);

};

#endif // SOCKETCANPORT_H
