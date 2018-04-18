#ifndef DEVICECHAIN_H
#define DEVICECHAIN_H

#include "Cia402device.h"
#include "SocketCanPort.h"

class DeviceChain
{
public:
    DeviceChain(string actualPort);
    DeviceChain(string actualPort, vector<long> ids);

    long SetPositions(vector<double> jointAngles);
    long SetupPositionMode(const uint32_t velocity,const uint32_t acceleration);
private:
    std::vector <CiA402Device> Joints;
    std::vector <SocketCanPort> Ports;
};

#endif // DEVICECHAIN_H
