#ifndef PORTBASE_H
#define PORTBASE_H


class PortBase
{
public:
    PortBase();

    int getPortFD();

protected:
    int portFD;

};

#endif // PORTBASE_H
