#ifndef CIA402DEVICE_H
#define CIA402DEVICE_H
#include "CiA402DeviceICanbus.h"
#include "CiA301CommPort.h"
#include "ObjectDictionary.h"
#include "PortBase.h"

class CiA402Device
{
public:

    CiA402Device();
    int CheckStatus();

    /**
     * @brief SwitchOn: Turn on the device and wait for commands.
     * @return: 0 if correct, negative on errors.
     */
    long SwitchOn();

    /**
     * @brief SwitchOff: Turn on the device.
     * @return: 0 if correct, negative on errors.
     */
    long SwitchOff();

    /**
     * @brief GetPosition: Get the position of the cia 402 device
     * @return: Position (angle in [rad])
     */
    double GetPosition();

    long SetCommunications(CiA301CommPort *newCommunications);
private:

    //CiA402DeviceICanbus interf;
    CiA301CommPort * comm;

};

#endif // CIA402DEVICE_H
