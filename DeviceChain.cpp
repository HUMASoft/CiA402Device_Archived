#include "DeviceChain.h"

DeviceChain::DeviceChain(string actualPort)
{
    Ports.resize(6);
    Joints.resize(6);

    for(int i=0; i<6; i++){
        Ports[i]=SocketCanPort(actualPort);
    }
    for(int i=0; i<6; i++){
        Joints[i]=CiA402Device(i, &Ports[i]);
    }
}

DeviceChain::DeviceChain(string actualPort, vector<long> ids)
{
    Ports.resize(6);
    Joints.resize(6);

    for(int i=0; i<6; i++){
        Ports[i]=SocketCanPort(actualPort);
    }
    for(int i=0; i<6; i++){
        Joints[i]=CiA402Device(ids[i], &Ports[i]);
    }

}
long DeviceChain::SetPositions(vector<double> jointAngles){

    for(int i=0; i<6; i++){
        Joints[i].SetPosition(jointAngles[i]);
    }
    return 0;
}

long DeviceChain::SetupPositionMode(const uint32_t velocity,const uint32_t acceleration){
    for(int i=0; i<6; i++){
        Joints[i].SetupPositionMode(velocity, acceleration);
    }
    return 0;
}
