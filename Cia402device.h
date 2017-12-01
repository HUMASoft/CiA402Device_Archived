#ifndef CIA402DEVICE_H
#define CIA402DEVICE_H
//#include "CiA402DeviceICanbus.h"
#include "CiA301CommPort.h"
#include "ObjectDictionary.h"
#include "PortBase.h"

class CiA402Device : public CiA301CommPort
{
public:

    CiA402Device();
    CiA402Device(uint8_t new_id);
    CiA402Device(uint8_t new_id, int fdPort);
    //CiA402Device(uint8_t new_id, CiA301CommPort *new_comm);
    /**
     * @brief CheckStatus: Returns the status word.
     * @return: status
     */
    uint16_t CheckStatus();

    /**
     * @brief PrintStatus: Print the status word.
     * @return: 0 if correct, negative on errors.
     */
    void PrintStatus();

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
     * @brief QuickStop: Fast turn off of the device.
     * @return: 0 if correct, negative on errors.
     */
    long QuickStop();

    /**
     * @brief GetPosition: Get the position of the cia 402 device
     * @return: Position (angle in [rad])
     */
    double GetPosition();

    long SetCommunications(int fdPort);
    int CheckError();
    bool setPositionMode();
    bool setVelocityModeRaw();
    long OperationMode(const vector<uint8_t> mode);
    long SetupPositionMode(const vector<uint32_t> target, const vector<uint32_t> velocity, const vector<uint32_t> acceleration, const vector<uint32_t> deceleration);
    long Setup_Velocity_Mode(const vector<uint8_t> target, const vector<uint8_t> acceleration);
    long ForceSwitchOff();

    //long SetupPositionMode(const vector<uint8_t> target, const vector<uint8_t> velocity, const vector<uint8_t> acceleration, const vector<uint8_t> deceleration);
    long SetPosition(uint32_t target);
    long SetupPositionMode(const uint32_t velocity, const uint32_t acceleration);
private:

    //methods
    long Init(uint8_t new_id);

    //properties
    int comm; //port file de
    //unsigned int id;
    double currentPosition;

};

#endif // CIA402DEVICE_H
