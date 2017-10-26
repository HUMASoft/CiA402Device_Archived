#ifndef CIA301COMMPORT_H
#define CIA301COMMPORT_H


class CiA301CommPort
{
public:
    CiA301CommPort(int newPortFileDescriptor);

private:
    int portFileDescriptor;

};

#endif // CIA301COMMPORT_H
