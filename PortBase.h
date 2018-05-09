#ifndef PORTBASE_H
#define PORTBASE_H

#include <cstdint>
#include <unistd.h>

#include <sys/socket.h>
#include <poll.h>
#include <sys/ioctl.h>

#include <linux/can.h>

class PortBase
{
public:
    PortBase();


    int getPortId();
    long FlushMsg();

    virtual long SetFilter(uint32_t canId, uint32_t mask)=0;
    virtual long GetMsg(uint32_t & canId, uint8_t * data, uint8_t &size)=0;
    virtual long PutMsg(const uint32_t & canId, uint8_t * const data, const uint8_t size)=0;
    virtual long GetNMT(uint8_t *data, uint8_t & size)=0;




protected:
    int portId;
    int portType; //type of port: 1 chardev, 2 network socket

    int portNMT;
    can_frame frame;
    long buffSizeId;
    long buffSizeNMT;


};

#endif // PORTBASE_H
