#ifndef PORTBASE_H
#define PORTBASE_H


class PortBase
{
public:
    PortBase();

    virtual int getPortFileDescriptor()=0;

protected:
    int portFileDescriptor;

};

#endif // PORTBASE_H
