#ifndef PORTBASE_H
#define PORTBASE_H

#include <cstdint>

class PortBase
{
public:
    PortBase();


    int getPortFD();

    virtual long SetFilter(uint32_t canId, uint32_t mask)=0;
    virtual long GetMsg(uint32_t & canId, uint8_t * data, uint8_t size)=0;
    virtual long PutMsg(const uint32_t & canId, uint8_t * const data, const uint8_t size)=0;


protected:
    int portFD;


};

#endif // PORTBASE_H
