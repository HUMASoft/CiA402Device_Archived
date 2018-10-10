#include "PortBase.h"

PortBase::PortBase()
{

}

int PortBase::getPortId()
{
    return portId;

}

long PortBase::FlushMsg()
{
    for (buffSizeId; buffSizeId>0; buffSizeId--)
    {
        read(portId, &frame, sizeof(struct can_frame));
    }
    return 0;

}
