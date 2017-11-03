#include "Cia402device.h"

long CiA402Device::Init(uint8_t new_id)
{
    //id = new_id;
    return 0;
}

CiA402Device::CiA402Device() : CiA301CommPort(1,1) //stdout
{

    comm = 1; //stdout

}

CiA402Device::CiA402Device(uint8_t new_id) : CiA301CommPort(1, new_id)
{

    comm = 1; //stdout
}

CiA402Device::CiA402Device(uint8_t new_id, int fdPort) : CiA301CommPort(fdPort, new_id)
{
    comm = fdPort;

}

long CiA402Device::SwitchOn(){

WriteNMT(od::reset);
sleep(1);
WritePDO(od::readytoswitchon);
sleep(1);
WritePDO(od::switchon);
sleep(2);
WritePDO(od::enable);

    return 0;
}

int CiA402Device::CheckStatus()
{
    //uint16_t* statusp;
    uint16_t status;
    vector<uint8_t> data={0x40};
    data.push_back(od::statusword[0]);
    data.push_back(od::statusword[1]);

    //Ask for the status word
    status = (uint16_t) ReadSDO(data);

    cout << "status word: " << status << endl;
    //Print decoded response for status word
    switch (status)
    {

    case 1: //
        break;

    //default:
    }

    return 0;
}


long CiA402Device::SwitchOff()
{

    return 0;
}

double CiA402Device::GetPosition()
{
    return 0.53; //fix this

}

long CiA402Device::SetCommunications(int fdPort)
{
    comm = fdPort;
    return 0;
}

