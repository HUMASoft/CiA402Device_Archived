#ifndef CIA402DEVICE_H
#define CIA402DEVICE_H
#include "CiA402DeviceICanbus.h"

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
private:

    CiA402DeviceICanbus interf;

};

#endif // CIA402DEVICE_H
