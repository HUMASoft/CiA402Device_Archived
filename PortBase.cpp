#include "PortBase.h"

PortBase::PortBase()
{

}

int PortBase::getPortFileDescriptor() const
{
    return portFileDescriptor;
}
