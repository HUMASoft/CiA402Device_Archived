#include "Cia402device.h"

vector<uint8_t> data32to4x8(uint32_t in);


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

CiA402Device::CiA402Device(uint8_t new_id, PortBase *new_port) : CiA301CommPort(new_port, new_id)
{


}

long CiA402Device::Reset()
{
     cout<<"RESET"<<endl;

     double response;
     //response = WriteNMT(od::reset);
     WriteNMT(od::fullreset);
     //Wait needed for reset to finish. Using sleep
     sleep(3);

     //Wait Start finish NMT [01 id]
     //ReadNMT(nmt::started);

     //reset response 700+id -->booting
     ReadErrorNMT();

     cout<<"START NODE"<<endl;
     WriteNMT(od::start);

     FlushBuffer();
return 0;
}


long CiA402Device::StartNode()
{

     cout<<"START NODE"<<endl;
     WriteNMT(od::start);
     return 0;

}

long CiA402Device::SwitchOn()
{
    //Watch out!! sleep commands removed!!!


    //TODO: Check the aim of this part of code.

//        int i=0;
//        int e=0; //marks transmission errors
//        uint16_t status;
//     status = CheckStatus()&0x6f; //mask 01101111=6f
//     while (i<3){   //three tries to receive the correct message
//        if (status == 0x00) {
//             cout<<"response" << response <<endl;
//             i=3;
//             e=0;
//        }
//        else {
//             sleep(1);
//             i++;
//             if (i=3){
//                 e=1;
//             }
//        }
//     }
//     if (e=1){
//         cout<<"Error. Wrong message or message not received"<<endl;
//         cerr << "Start Failed. Try node init Before with reset. " << endl;
//         //PrintStatus();
//         return 0;
//        }
//     else {
//         i=0;
//     }


    long response;

     cout<<"READYTOSWITCHON"<<endl;
     WritePDO(od::goreadytoswitchon);

     //it is the same for all pdos??
     response = ReadPDO(0);
     FlushBuffer();
//     response = ReadPDO(1);
     cout<<"READYTOSWITCHON RESPONSE: " << response <<endl;

     cout<<"SWITCHON"<<endl;
     response = WritePDO(od::goswitchon);
cout<<"SWITCHON WritePDO RESPONSE" << response <<endl;
     //cout<<"response" << response <<endl;
     //FlushBuffer(2); //remove two messages, pdo1tx and pdo2tx
     //listen pdo1tx and pdo2tx
     //it is the same for all pdos??
     response = ReadPDO(0);
     FlushBuffer();


     cout<<"SWITCHON RESPONSE" << response <<endl;

     cout<<"ENABLE"<<endl;
     response = WritePDO(od::goenable);
     sleep(1);

     //cout<<"response" << response <<endl;
     //FlushBuffer(2); //remove two messages, pdo1tx and pdo2tx
     //listen pdo1tx and pdo2tx
     //it is the same for all pdos??
     response = ReadPDO(0);
     FlushBuffer();


     //dont use it here. Call from main.
//     cout<<"response"<< response <<endl;
//     OperationMode(od::positionmode);
     sleep(1);

    return 0;
}

long CiA402Device::OperationMode(const vector<uint8_t> new_mode)
{
    //FlushBuffer();

    long tmpmode;

    tmpmode = ReadSDO(od::OperationModeDisplay);

    cout<<"Changing from OperationMode : " << tmpmode <<endl;

    //ask the node for write proper mode in 6060 address
    WriteSDO(od::OperationMode,new_mode);

    //TODO: Change WriteSDO to readCobId its acks.
    //write responses with ack
//    FlushBuffer(1);

    tmpmode = ReadSDO(od::OperationModeDisplay);

    cout<<"To OperationMode : " << tmpmode <<endl;

    return 0;
}


uint16_t CiA402Device::CheckStatus()
{
    //uint16_t* statusp;
    uint16_t status;
    vector<uint8_t> data = od::statusword;
//    data.push_back(od::statusword[0]);
//    data.push_back(od::statusword[1]);

    //Ask for the status word
    status = (uint16_t) ReadSDO(data);
    //status = 0x07;
    //filter state

    return status;

}
void CiA402Device::PrintStatus()
{
//    //uint16_t* statusp;
   uint16_t status;
    //vector<uint8_t> data={0x40};
    vector<uint8_t> data=od::statusword;
//    data.push_back(od::statusword[0]);
//    data.push_back(od::statusword[1]);

    //Ask for the status word
    status = (uint16_t) ReadSDO(data);
    cout << "status word: " << std::bitset<16>(status)<< endl;

//    status = 0x024; //testing data
    //filter state
    status &= 0x6f; //mask 01101111=6f
    //cout << "status word with mask: " << std::bitset<16>(status)<< endl;

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
    //FlushBuffer();
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

long CiA402Device::ForceSwitchOff(){
    WritePDO(od::goswitchondisable);
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

//double CiA402Device::GetPosition()
//{

//    return (uint32_t) ReadSDO(od::positionaddress)*360/4096;//*360000/4096

//}

double CiA402Device::GetPosition()
{

    int32_t position= (int32_t)ReadSDO(od::positionaddress);

    //cout<<"POS --- "<<position<<endl;
    return (double)position*360/15155;//*360000/4096
//       double position = ReadSDO(od::positionaddress)*360/15155;
//       while(position > 1200){
//             position = ReadSDO(od::positionaddress)*360/15155;
//       }
//       cout<<"DENTRO DE GETPOSITIOB"<<position<<endl;
//    //return (uint32_t) ReadSDO(od::positionaddress)*360/4096;//*360000/4096
//    return position;

}

double CiA402Device::GetVelocity()
{


    int v = (int)ReadSDO(od::velocityaddress)*15/65536;
    double ret = v/3.7;
    //cout<<"Get_Velocity"<<ret<<"rpm"<<endl;
    return ret;



}

long CiA402Device::SetCommunications(int fdPort)
{
    comm = fdPort;
    return 0;
}

long CiA402Device::SetupPositionMode(/*const vector<uint8_t> target,*/const uint32_t velocity,const uint32_t acceleration /*const vector<uint8_t> deceleration*/){

int32_t velocityr;
int32_t accelerationr;
    OperationMode(od::positionmode);
    /* Converts from degrees/s to lines per ms, and then this value is shifted two bytes
 to the left to form the high part of the message. Then the low part is added.
 The high part corresponds to the increments/sample while the low part corresponds
 to the subdivion of an increment */
    velocityr=((velocity*4096/360000)<<16)+0;

    // Motion profile type -  trapezoidal
    //WriteSDO(od::motion_profile_type,od::linear_ramp_trapezoidal);
    //Si paso los parametros convertidos en ui, sino convertir primero
    WriteSDO(od::profile_velocity,data32to4x8(velocityr));
    //Si paso los parametros convertidos en ui, sino convertir primero
    accelerationr=((acceleration*4096/360000000)<<16)+0;
    WriteSDO(od::profile_acceleration,data32to4x8(accelerationr));
//  The target position is the position that the drive should move to in
//  position profile mode using the current settings of motion control parameters
//  such as velocity, acceleration, and motion profile type etc.
//  It is given in position units.
//  Si paso los parametros convertidos en ui, sino convertir primero
    //WriteSDO(od::target_position,target);
    //WriteSDO(od::quick_stop_deceleration,deceleration);
//    FlushBuffer();
   return 0;
}

long CiA402Device::Setup_Velocity_Mode(const uint32_t target,const uint32_t acceleration){
//    In the Velocity Profile Mode the drive performs speed control.
//    The built-in reference generator computes a speed profile with a trapezoidal shape,
//    due to a limited acceleration. The Target Velocity object specifies
//    the jog speed (speed sign specifies the direction) and the Profile Acceleration
//    object the acceleration/deceleration rate.

    OperationMode(od::velocitymode);
    int32_t t=(target*256/15)<<16;
    int32_t accelerationr;

//    The target velocity is the input for the trajectory generator
//    and the value is given in user-defined velocity units.
//    Si paso los parametros convertidos en ui, sino convertir primero

    //targetr=( (target*(4096/360)*(1) ) << 16 )+0;//4096/360= [encoder-steps/deg] and 1000 [ms] in a [s]

     WriteSDO(od::target_velocity,data32to4x8(t));

    //accelerationr=((acceleration*4096/360000000))+0;
    accelerationr=acceleration*65536;
    WriteSDO(od::profile_acceleration,data32to4x8(accelerationr));
    return 0;
}

///
/// \brief CiA402Device::SetPosition This function performs the
/// communication with the device to perform a position control in
/// a motor and perform a motion towards a selected position. The motor remembers
/// as position 0 the position in which it has been switched on, always returning to
/// this position when its target is 0.
/// \param target of type uint32_t is the desired position by means of the enconder
/// lines. If this value is bigger than the number of lines of the enconder used,
/// the motor's axis will perform more than one turn.
/// \return This function returns a variable of type long with value 0.
///

long CiA402Device::SetPosition(long target){

//WATCH SLEEPS!!!!!!!!!!!!
//remove when working code
    //vector<uint8_t> value;
    //convert target to value
    WritePDO(od::goenable);
    FlushBuffer();
    //sleep(1);
    long target_t=(long)target*3.7*4096/360;

    WriteSDO(od::target_position,data32to4x8(target_t));
//    FlushBuffer();
    //WritePDO4(data32to4x8(target));
    //sleep(1);
    long pos = ReadSDO(od::target_position);
    cout<<"target_position: "<<pos<<endl;
    FlushBuffer();
    //sleep(1);

    //lectura del status word y comprobar target reached (posicion bit11 = 1)
    //cada vez que la statusword cambia, envía un mensaje pdo.
    //revisar esto para esperar el mensaje correcto.
    long stat = ReadSDO(od::statusword);
    cout<<"statusword: "<<stat<<endl;
    //FlushBuffer();
    //sleep(1);

    //setup via control word
//    vector<uint8_t>cw={0x30,0x08,0x00 ,0x00 };
//    WritePDO(cw);
//    sleep(1);01100100
    cout<<"RUN"<<endl;
      WritePDO(od::run);
      FlushBuffer();
    return 0;
}

long CiA402Device::SetVelocity(double target){


    int32_t targetr;
//targetr=( (1) << 16 );
   //targetr=( (target*(4096/360)*(2) ) << 16 );*///4096/360= [encoder-steps/deg] and 1000 [ms] in a [s]
    //WriteSDO(od::target_velocity,data32to4x8(targetr));
//    accelerationr=((acceleration*4096/360000000)<<16)+0;

//    WriteSDO(od::profile_acceleration,data32to4x8(accelerationr));
//    //vector<uint8_t> value;
//    //convert target to value
//    WritePDO(od::goenable);
//    FlushBuffer();
//    //sleep(1);

//    WriteSDO(od::target_velocity,data32to4x8(target));
////    FlushBuffer();
//    //WritePDO4(data32to4x8(target));
//    //sleep(1);
//    long vel = ReadSDO(od::target_velocity);
//    cout<<"target_velocity: "<<vel<<endl;
//    FlushBuffer();
//    //sleep(1);

//    //lectura del status word y comprobar target reached (posicion bit11 = 1)
//    //cada vez que la statusword cambia, envía un mensaje pdo.
//    //revisar esto para esperar el mensaje correcto.
//    long stat = ReadSDO(od::statusword);
//    cout<<"statusword: "<<stat<<endl;
//    //FlushBuffer();
//    //sleep(1);
//    targetr=((target*1024)/6.28);
//    uint32_t t=(targetr<<16)+0;
//    uint32_t t=(target*256/15)<<16;
    double ui=target*0x10000*4096/60000;
    int32_t t=(int32_t)ui;
    WriteSDO(od::target_velocity,data32to4x8(t));


//    //setup via control word
////    vector<uint8_t>cw={0x30,0x08,0x00 ,0x00 };
////    WritePDO(cw);
////    sleep(1);
//    cout<<"RUN"<<endl;
//      WritePDO(od::run);
//      FlushBuffer();
    return 0;
}

///
/// \brief CiA402Device::DegreeConv This function converts a variable of type uint32_t
/// which contains the position of a motor with an encoder of 4096 lines in decimal degrees
/// into the equivalent position as function of the enconders lines. If the parameter
/// has a value of 360 the function will return an uint32_t variable with a value of
/// 4096.
/// \param DegreeTarget A uint32_t variable with the desired position in decimal degrees
/// \return targetPos A uint32_t variable with the equivalent position as a function
/// of the encoder lines.
///



long CiA402Device::Setup_Torque_Mode()
{

    cout << "Setup_Torque_Mode " <<endl;
    WriteSDO(od::torque_type_extern,od::torque_online);

    OperationMode(od::torquemode);

    return 0;
}

long CiA402Device::SetTorque(double target){


    int32_t targetr=(int32_t)target*0x10000;

    if ((target < -1000))
    {
        targetr=-1000*0x10000;
    }
    if ((target > 1000))
    {
        targetr=1000*0x10000;
    }


//    cout<< "targetr " <<targetr;

    WriteSDO(od::torque_target,data32to4x8(targetr));

    //cout<<"RUN"<<endl;
    WritePDO(od::run);
    FlushBuffer();
    return 0;
}

vector<uint8_t> data32to4x8(uint32_t in)
{
//    cout<<"---------------------------"<<endl;
    vector<uint8_t> retvalue(4);
    retvalue[0] = in&0x000000FF;
    retvalue[1] = (in&0x0000FF00)>>8;
    retvalue[2] = (in&0x00FF0000)>>16;
    retvalue[3] = (in&0xFF000000)>>24;
//    cout<< " " <<(int)retvalue[0]
//        << " ," <<(int)retvalue[1]
//        << " , "<<(int)retvalue[2]
//        << " , "<<(int)retvalue[3]<<endl;
    return retvalue;

}
