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

long CiA402Device::SwitchOn()
{

//Watch out!! see how to remove sleep commands!!!

//   const vector<u_int8_t> obj2111 ={0x40,0x11,0x21,0x00,0x00};

    cerr<<"RESET"<<endl;
    WriteNMT(od::reset);
    sleep(1);
    FlushBuffer();
    //OperationMode(od::positionmode);
    cerr<<"START"<<endl;
    WriteNMT(od::start);
    sleep(1);
    FlushBuffer();
    //FlushBuffer();
    WritePDO(od::readytoswitchon);
    sleep(1);
    FlushBuffer();

    sleep(1);
   cerr<<"SWITCHON"<<endl;
    WritePDO(od::switchon);
    sleep(1);
    FlushBuffer();
  cerr<<"ENABLE"<<endl;
    WritePDO(od::enable);
    FlushBuffer();
   cerr<<"RUN"<<endl;
     WritePDO(od::run);
     FlushBuffer();
    return 0;
}

long CiA402Device::OperationMode(const vector<uint8_t> mode)
{
    //ask the node for write proper mode in 6060 address
    WriteSDO(od::OperationMode,mode);
    //wait the answer (tx0(580)+id)


    return 0;
}


uint16_t CiA402Device::CheckStatus()
{
    //uint16_t* statusp;
    uint16_t status;
    vector<uint8_t> data={0x40};
    data.push_back(od::statusword[0]);
    data.push_back(od::statusword[1]);

    //Ask for the status word
    status = (uint16_t) ReadSDO(data);
    //status = 0x07;
    //filter state

    return status;

}
void CiA402Device::PrintStatus()
{
    //uint16_t* statusp;
    uint16_t status;
    vector<uint8_t> data={0x40};
    data.push_back(od::statusword[0]);
    data.push_back(od::statusword[1]);

    //Ask for the status word
    status = (uint16_t) ReadSDO(data);
    cout << "status word: " << std::bitset<16>(status)<< endl;

   // status = 0x07; //testing data
    //filter state
    status = status&0x6f; //mask 01101111=6f

    switch(status)
    {
    case 0x00:
        cout<<"Not Ready to switch on"<<endl;
        break;
    case 0x60:
    case 0x40:
        cout<<"Switch on disabled"<<endl;
        break;
    case 0x21:
        cout<<"Ready to switch on"<<endl;
        break;
    case 0x23:
        cout<<"Switched on"<<endl;
        break;
    case 0x27:
        cout<<"Operation enabled"<<endl;
        break;
    case 0x07:
        cout<<"Quick stop active"<<endl;
        break;
    case 0x0f:
    case 0x2f:
        cout<<"Fault reaction active"<<endl;
        break;
    case 0x08:
    case 0x28:
        cout<<"Fault"<<endl;
        break;
    default:
        cout<<"Not known"<<endl;

    }
   return;
}

int CiA402Device::CheckError()
{
    //uint16_t* statusp;
    uint32_t error;
    vector<uint8_t> data={0x40};
    data.push_back(od::checkerror[0]);
    data.push_back(od::checkerror[1]);

    //Ask for the status word
    error = (uint32_t) ReadSDO(data);

    cout << "checkerror: " << error << endl;
    cout << "checkerror: " << std::bitset<32>(error)<< endl;
    //Print decoded response for status word
    switch (error)
    {

    case 1: //
        break;

    //default:
    }

    return 0;
}

long CiA402Device::SwitchOff()
{
    uint16_t status;
    status=CheckStatus();
    //filter state
    status = status&0x6f; //mask 01101111=6f

    switch(status)
    {
//    case 0x00:
//        cout<<"Not Ready to switch on"<<endl;
//        break;
//    case 0x60:
//    case 0x40:
//        cout<<"1"<<endl;
//        break;
    case 0x21:
        //cout<<"2"<<endl;
        cout<<"7"<<endl;
        WritePDO(od::quickstop);
        FlushBuffer();
        break;
    case 0x23:
        cout<<"10"<<endl;
        //WritePDO(od::readytoswitchon);
        //FlushBuffer();
        WritePDO(od::quickstop);
        FlushBuffer();
        break;
    case 0x27:
        cout<<"Leaving Operation enabled"<<endl;
      /*  WritePDO(od::switchon);
        FlushBuffer();
        WritePDO(od::readytoswitchon);
        FlushBuffer();*/
        cout<<"11"<<endl;
        WritePDO(od::quickstop);
        FlushBuffer();
        cout<<"12"<<endl;
        WriteNMT(od::start);
        break;
    case 0x07:
        cout<<"5"<<endl;
        WritePDO(od::quickstop);
        FlushBuffer();
        break;
    case 0x0f:
    case 0x2f:
        cout<<"Fault reaction active"<<endl;
        break;
    case 0x08:
    case 0x28:
        cout<<"Fault"<<endl;
        break;
    default:
        cout<<"Not known"<<endl;

    }
    return 0;
}

long CiA402Device::QuickStop()
{
    const vector<uint8_t> mode = {0x00};
    WriteSDO(od::quick_stop_mode,mode);
    sleep(1);
    WriteSDO(od::stop_option_code,mode);
    sleep(1);
    cout<<"SwitchOff"<<endl;
    WritePDO(od::quickstop);
    FlushBuffer();

    return 0;
}

double CiA402Device::GetPosition()
{
    return (uint32_t) ReadSDO(od::positionaddress);

}

long CiA402Device::SetCommunications(int fdPort)
{
    comm = fdPort;
    return 0;
}

long CiA402Device::Setup_Position_Mode(const vector<uint8_t> target,const vector<uint8_t> velocity,const vector<uint8_t> acceleration,const vector<uint8_t> deceleration){
    OperationMode(od::positionmode);
    // Motion profile type -  trapezoidal
    WriteSDO(od::motion_profile_type,od::linear_ramp_trapezoidal);
    //Si paso los parametros convertidos en ui, sino convertir primero
    WriteSDO(od::profile_velocity,velocity);
    //Si paso los parametros convertidos en ui, sino convertir primero
    WriteSDO(od::profile_acceleration,acceleration);
//  The target position is the position that the drive should move to in
//  position profile mode using the current settings of motion control parameters
//  such as velocity, acceleration, and motion profile type etc.
//  It is given in position units.
//  Si paso los parametros convertidos en ui, sino convertir primero
    WriteSDO(od::target_position,target);
    WriteSDO(od::quick_stop_deceleration,deceleration);
   return 0;
}

long CiA402Device::Setup_Velocity_Mode(const vector<uint8_t> target,const vector<uint8_t> acceleration){
//    In the Velocity Profile Mode the drive performs speed control.
//    The built-in reference generator computes a speed profile with a trapezoidal shape,
//    due to a limited acceleration. The Target Velocity object specifies
//    the jog speed (speed sign specifies the direction) and the Profile Acceleration
//    object the acceleration/deceleration rate.
    OperationMode(od::velocitymode);
//    The target velocity is the input for the trajectory generator
//    and the value is given in user-defined velocity units.
//    Si paso los parametros convertidos en ui, sino convertir primero
    WriteSDO(od::target_velocity,target);
    WriteSDO(od::profile_acceleration,acceleration);
    return 0;
}
