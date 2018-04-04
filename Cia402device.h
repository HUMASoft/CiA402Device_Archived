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
    //set the filter to port so commport will only accept messages to his id
    //must decide to make not constant. Till then, moved up to main
    //Dropping constant call for now on port. Review later.
    CiA402Device(uint8_t new_id, /*const*/ PortBase *new_port);
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
    double GetVelocity();

    long SetCommunications(int fdPort);
    int CheckError();
    bool setPositionMode();
    bool setVelocityModeRaw();
    long OperationMode(const vector<uint8_t> new_mode);
    long SetupPositionMode(const vector<uint32_t> target, const vector<uint32_t> velocity, const vector<uint32_t> acceleration, const vector<uint32_t> deceleration);
    long Setup_Velocity_Mode(const uint32_t target, const uint32_t acceleration);
    long Setup_Torque_Mode();
    long SetTorque(uint32_t target);
    long ForceSwitchOff();

    //long SetupPositionMode(const vector<uint8_t> target, const vector<uint8_t> velocity, const vector<uint8_t> acceleration, const vector<uint8_t> deceleration);
    long SetPosition(long target);
    long SetupPositionMode(const uint32_t velocity, const uint32_t acceleration);
    ///
    /// \brief Reset This function resets the node corresponding to this object.
    /// \return a long of value 0 if there are no errors.
    ///
    long Reset();

    long StartNode();

    long SetVelocity(uint32_t target);
private:

    //methods
    long Init(uint8_t new_id);

    //properties
    int comm; //port file de
    //unsigned int id;
    double currentPosition;
    int useport;
    PortBase* port;

};

#endif // CIA402DEVICE_H
