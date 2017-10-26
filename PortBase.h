#ifndef PORTBASE_H
#define PORTBASE_H


class PortBase
{
public:
    PortBase();

    int getPortFileDescriptor() const;

protected:
    int portFileDescriptor;

};

#endif // PORTBASE_H
