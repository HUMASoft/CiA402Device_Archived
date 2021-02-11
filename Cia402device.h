#ifndef CIA402DEVICE_H
#define CIA402DEVICE_H

#include <chrono>
#include <thread>
#include <math.h>       /* signbit, sqrt */
//#include "CiA402DeviceICanbus.h"
#include "CiA301CommPort.h"
#include "SocketCanPort.h"
#include "ObjectDictionary.h"
#include "PortBase.h"
#include "CiA402SetupData.h"
#include "SocketCanPort.h"


#define PI 3.14159265358
#define RPM2RADS (2.0*PI)/60.0
#define RADS2RPM 60.0/(2.0*PI)
#define DEG2RADS (2.0*PI)/360.0
#define RADS2DEG 360.0/(2.0*PI)


/*If no factor is applied, the profile velocity object receives data as a FIXED32 variable.
the high part represents encoder increments/sample, and the low part represents a subdivision of an increment.
So 65536(0x00010000) = 1 encoder increment/sample. The minimum speed is 1 encoder increment/sample.*/
#define HIGHPART_BITSHIFT_16 0x10000

#define ANALOGUE_INPUT_SCALE 0xFFF0

//using namespace std::chrono;

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
    CiA402Device(uint8_t new_id, PortBase *new_port, CiA402SetupData *deviceData);
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
    double GetFilteredVelocity(int samples);
    double GetMeanVelocity();
    double GetAmps();
    double GetFilterdAmps();

    long SetCommunications(int fdPort);
    int CheckError();
//    bool setPositionMode();
//    bool setVelocityModeRaw();
    long OperationMode(const vector<uint8_t> new_mode);
    long Setup_Velocity_Mode(const uint32_t acceleration=1, const uint32_t target=0);
    long Setup_Torque_Mode();
    long SetTorque(double target);
    long SetAmpRaw(double target);
    long ForceSwitchOff();


    //long SetupPositionMode(const vector<uint8_t> target, const vector<uint8_t> velocity, const vector<uint8_t> acceleration, const vector<uint8_t> deceleration);
    long SetPosition(double target);
    long SetupPositionMode(const uint32_t velocity=1, const uint32_t acceleration=1);
//    long SetupPositionMode(const vector<uint32_t> target, const vector<uint32_t> velocity, const vector<uint32_t> acceleration, const vector<uint32_t> deceleration);
    long SetPositionRECURSIVE_test(long target);
    long SetTarget_VELOCITY_PROPORCIONAL(double target, float kp);

    /// \brief Reset This function resets the node corresponding to this object.
    /// \return a long of value 0 if there are no errors.
    ///
    long Reset();

    long StartNode();

    long SetVelocity(double target);

    long SetEnc_res(int lines);

    long SetRed_Mot(float reduction_ratio);

    long SetSampling_period(float sampling_period);

    long Scaling(void);
private:

    //methods
    long Init(CiA402SetupData *deviceData);

    //properties
    int comm; //port file de
    //unsigned int id;

//    PortBase* port;

    float reduction_ratio_motor;    // Transmission ratio between the motor displacement in SI units and load displacement
    int encoder_resolution;         // Nº lines for incremental encoder quadrature  (lines X 4)
    float SampSL;                   // speed/position loop sampling period of the motor Control (sampling_slow_loop)
    int current_limit;              // current_limit. CAnOpen programming iPOS 5.5.7. Object 207Fh: Current limit

   //Technosoft drives work with parameters and variables represented in the drive internal units (IU).
   // Constant to convert velocity from rad to count/sample
   // The internal speed units are internal position units/(slow loop sampling period)
    float Scaling_Factors_Velocity;
    float Scaling_Factors_Position;
    float Scaling_Factors_Acceleration;

    //GetMeanVelocity variables
    double currentPosition, lastPosition;
    double meanVelocity;
    std::chrono::system_clock::time_point actualTimeValue, lastTimeValue; //last time value
    std::chrono::system_clock::time_point encoderChangeTime;
    double encoderSpan;

    std::chrono::nanoseconds dtsWait, tWaited;


};

#endif // CIA402DEVICE_H
