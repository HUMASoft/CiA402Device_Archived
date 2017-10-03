#ifndef CIA402DEVICE_H
#define CIA402DEVICE_H
#include "CiA402DeviceICanbus.h"

class CiA402Device
{
public:

    CiA402Device();
    int SwitchOn();
    int CheckStatus();
private:

    CiA402DeviceICanbus interf;

};

#endif // CIA402DEVICE_H
