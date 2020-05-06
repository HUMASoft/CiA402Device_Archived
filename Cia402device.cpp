#include "Cia402device.h"

vector<uint8_t> data32to4x8(uint32_t in);

vector<uint8_t> data16to2x8(uint16_t in);

long CiA402Device::Init(CiA402SetupData *deviceData)
{


    Scaling_Factors_Position = deviceData->getScaling_Factors_Position();
    Scaling_Factors_Velocity = deviceData->getScaling_Factors_Velocity();
    Scaling_Factors_Acceleration= deviceData->getScaling_Factors_Acceleration();

    //Initialize velocity and position
    encoder_resolution = deviceData->getEncRes();
    meanVelocity = 0;
    currentPosition = 0;
    lastPosition = 0;
    encoderSpan = 0;

    tWaited = chrono::milliseconds(1);
    ///set adress for filtered amps (not working)
//    const vector<uint8_t> cutoff = {0xFF,0xFF};
//    WriteSDO(od::filtered_amps_set_cutoff,cutoff);

//    lastTimeValue = actualTimeValue;
//    actualTimeValue = chrono::system_clock::now();
//    tWaited = actualTimeValue.time_since_epoch() - lastTimeValue.time_since_epoch(); //nanoseconds
//    tWaited = tWaited/1000000; //seconds



//    //Scaling();
//    float count_sec = encoder_resolution/60.0;
//    // Motor_Position[IU]=Scaling_factors_velocity*Load_Position[SI]
//    Scaling_Factors_Position = reduction_ratio_motor*encoder_resolution/(2.0*PI);

//    // Motor_Speed[IU]=Scaling_factors_velocity*Load_Speed[SI]
//    Scaling_Factors_Velocity = RADS2RPM*count_sec*reduction_ratio_motor*SampSL;
//    cout << "Scaling_Factors_Velocity - " << Scaling_Factors_Velocity << endl;

//    // Motor_Accel[IU]=Scaling_factors_velocity*Load_Accel[SI]
//    Scaling_Factors_Acceleration = Scaling_Factors_Velocity*SampSL;


    return 0;
}

CiA402Device::CiA402Device()
    : CiA301CommPort(1,1)
{
    CiA402SetupData defaultInit(2048,24,0.001, 0.144, 20 );
    Init(&defaultInit);
    comm = 1;

}

CiA402Device::CiA402Device(uint8_t new_id)
    : CiA301CommPort(1, new_id)
{
    CiA402SetupData defaultInit(2048,24,0.001, 0.144, 20 );
    Init(&defaultInit);
    comm = 1;
}

CiA402Device::CiA402Device(uint8_t new_id, int fdPort)
    : CiA301CommPort(fdPort, new_id)
{

    CiA402SetupData defaultInit(2048,24,0.001, 0.144, 20 );
    Init(&defaultInit);
    comm = fdPort;

}

CiA402Device::CiA402Device(uint8_t new_id, PortBase *new_port)
    : CiA301CommPort(new_port, new_id)
{
    CiA402SetupData defaultInit(2048,24,0.001, 0.144, 20 );
    Init(&defaultInit);
//    port=new_port;

}

CiA402Device::CiA402Device(uint8_t new_id, PortBase *new_port, CiA402SetupData *deviceData)
    : CiA301CommPort(new_port, new_id)
{
     Init(deviceData);
//     port=new_port;


}

long CiA402Device::Scaling(void)
{
    float count_sec = encoder_resolution/60.0;


    // Motor_Position[IU]=Scaling_factors_velocity*Load_Position[SI]
    Scaling_Factors_Position = reduction_ratio_motor*encoder_resolution/(2.0*PI);

    // Motor_Speed[IU]=Scaling_factors_velocity*Load_Speed[SI]
    Scaling_Factors_Velocity = RADS2RPM*count_sec*reduction_ratio_motor*SampSL;
    cout << "Scaling_Factors_Velocity - " << Scaling_Factors_Velocity << endl;

    // Motor_Accel[IU]=Scaling_factors_velocity*Load_Accel[SI]
    Scaling_Factors_Acceleration = Scaling_Factors_Velocity*SampSL;

    return 0;
}

long CiA402Device::Reset()
{

//     double response;
     //response = WriteNMT(od::reset);
     cout<<"RESET id: " << (uint)id << endl;

     WriteNMT(od::fullreset);


     //Wait Start finish NMT [01 id]
     //ReadNMT(nmt::started);

     //reset response 700+id -->booting
     ReadErrorNMT();
     //Wait needed for reset to finish. Using sleep
     sleep(1);

     WriteNMT(od::start);

     cout<<"NODE " << (uint)id << " STARTED"<<endl;

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


    EnablePDOs();

    long response;

     cout<<"GOREADYTOSWITCHON"<<endl;
     WritePDO1(od::goreadytoswitchon);

     //it is the same for all pdos??
     response = ReadPDO(0);
     FlushBuffer();
//     response = ReadPDO(1);
     cout<<"READYTOSWITCHON RESPONSE: " << response <<endl;

//     cout<<"SWITCHON"<<endl;
     response = WritePDO1(od::goswitchon);
//cout<<"SWITCHON WritePDO RESPONSE" << response <<endl;
     //cout<<"response" << response <<endl;
     //FlushBuffer(2); //remove two messages, pdo1tx and pdo2tx
     //listen pdo1tx and pdo2tx
     //it is the same for all pdos??
     response = ReadPDO(0);
     FlushBuffer();


//     cout<<"SWITCHON RESPONSE" << response <<endl;

     response = WritePDO1(od::goenable);
     cout<<"NODE " << (uint)id <<" ENABLED"<<endl;
     DisablePDOs();
     sleep(1);

     //cout<<"response" << response <<endl;
     //FlushBuffer(2); //remove two messages, pdo1tx and pdo2tx
     //listen pdo1tx and pdo2tx
     //it is the same for all pdos??
//     response = ReadPDO(0);

     FlushBuffer();


     //dont use it here. Call from main.
//     cout<<"response"<< response <<endl;
//     OperationMode(od::positionmode);
//     sleep(1);

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
    WritePDO1(od::goswitchondisable);
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
        WritePDO1(od::quickstop);
        FlushBuffer();
        break;
    case 0x23:
        cout<<"10"<<endl;
        //WritePDO(od::readytoswitchon);
        //FlushBuffer();
        WritePDO1(od::quickstop);
        FlushBuffer();
        break;
    case 0x27:
        cout<<"Leaving Operation enabled"<<endl;
      /*  WritePDO(od::switchon);
        FlushBuffer();
        WritePDO(od::readytoswitchon);
        FlushBuffer();*/
        cout<<"11"<<endl;
        WritePDO1(od::quickstop);
        FlushBuffer();
        cout<<"12"<<endl;
        WriteNMT(od::start);
        break;
    case 0x07:
        cout<<"5"<<endl;
        WritePDO1(od::quickstop);
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
    WritePDO1(od::quickstop);
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

//    cout<<"POS --- "<<position<<endl;
//    cout<<"reduction_ratio_motor*encoder_resolution "<<reduction_ratio_motor<< " "<<encoder_resolution<<" "<<reduction_ratio_motor*encoder_resolution<<endl;

    //cout<<"position: "<<position<<endl;
    double scale_position = (double)position;
    scale_position = scale_position / ( (double)Scaling_Factors_Position );

    return scale_position;
    //return (double)(position/Scaling_Factors_Position);//*360000/4096

//       double position = ReadSDO(od::positionaddress)*360/15155;
//       while(position > 1200){
//             position = ReadSDO(od::positionaddress)*360/15155;
//       } " "<<
//       cout<<"DENTRO DE GETPOSITIOB"<<position<<endl;
//    //return (uint32_t) ReadSDO(od::positionaddress)*360/4096;//*360000/4096
//    return position;

}

/// \brief CiA402Device::GetVelocity This function converts a variable of type uint32_t which contains the equivalent velocity
///  as function of the enconder lines/sample into the velocity of a motor with an encoder of X lines in radians per second.
///
/// \return This function returns a variable of type double with value the actual velocity in rad/s

double CiA402Device::GetVelocity()
{

    int32_t velocity= (int32_t)ReadSDO(od::velocityaddress);

    double scale_velocity = (double)velocity;
    scale_velocity = scale_velocity / ( (double)(Scaling_Factors_Velocity*HIGHPART_BITSHIFT_16) );
      // Motor_Position[IU]=Scaling_factors_velocity*Load_Position[SI];

//    cout << std::hex << "velocity  hex    - " <<  velocity <<endl;
//    cout << "velocity  float    - " << (double)velocity/(Scaling_Factors_Velocity*HIGHPART_BITSHIFT_16) << endl;
//    cout << "velocity           - " << velocity/(Scaling_Factors_Velocity*HIGHPART_BITSHIFT_16)<< endl;
//    cout << "velocity Low  - " << (velocity << 16) << endl;

    return scale_velocity;
    //return double(velocity/(Scaling_Factors_Velocity*HIGHPART_BITSHIFT_16));// /65536;
   // double ret = v/reduction_ratio_motor;
    //cout<<"Get_Velocity"<<ret<<"rpm"<<endl;++++++++++++++++++++++++++++
}

double CiA402Device::GetMeanVelocity()
{

    int32_t position= (int32_t)ReadSDO(od::positionaddress);

//    cout<<"POS --- "<<position<<endl;
//    cout<<"reduction_ratio_motor*encoder_resolution "<<reduction_ratio_motor<< " "<<encoder_resolution<<" "<<reduction_ratio_motor*encoder_resolution<<endl;

    //cout<<"position: "<<position<<endl;
    double scale_position = (double)position;

    lastPosition = currentPosition;
    currentPosition = scale_position / ( (double)Scaling_Factors_Position );

    if(lastPosition == currentPosition)
    {

        //keep old velocity value
        //check zero velocity
        if( chrono::system_clock::now() > encoderChangeTime)
        {
            meanVelocity = 0;
        }
    }
    else
    {
        //Compute velocity from position
        lastTimeValue = actualTimeValue;
        actualTimeValue = chrono::system_clock::now();
        tWaited = actualTimeValue.time_since_epoch() - lastTimeValue.time_since_epoch(); //nanoseconds
        tWaited = tWaited/1000000; //seconds
        meanVelocity = (currentPosition - lastPosition)/tWaited.count();
        //update zero check counter
        //that is, keeping actual velocity, prediction of the next encoder change
        encoderSpan = (2*M_PI/encoder_resolution)/meanVelocity;
        //time to check taking this encoder span
        encoderChangeTime = actualTimeValue + std::chrono::milliseconds(2*1000*long(encoderSpan));
    }
    //filter

    cout << "meanVelocity =  " << meanVelocity <<  ", currentPosition =  " << currentPosition <<  ", lastPosition =  " << lastPosition <<endl;
    cout << "tWaited =  " << tWaited.count() <<endl;


    return meanVelocity;
    //return double(velocity/(Scaling_Factors_Velocity*HIGHPART_BITSHIFT_16));// /65536;
   // double ret = v/reduction_ratio_motor;
    //cout<<"Get_Velocity"<<ret<<"rpm"<<endl;++++++++++++++++++++++++++++




}

double CiA402Device::GetFilteredVelocity(int samples)
{

    return 0;
}
double CiA402Device::GetAmps()
{

    int16_t amps= (int16_t)ReadSDO(od::getamps);

    double scaledamps = (double)amps;
    return scaledamps;
}
double CiA402Device::GetFilterdAmps()
{
    ///does not work yet

    int16_t amps= (int16_t)ReadSDO(od::filtered_amps);
    cout<<"Lectura int32 : "<<amps<<endl;
    double scaledamps = (double)amps;
    return scaledamps;
}

long CiA402Device::SetCommunications(int fdPort)
{
    comm = fdPort;
    return 0;
}

long CiA402Device::SetupPositionMode(const uint32_t velocity,const uint32_t acceleration /*const vector<uint8_t> deceleration*/){

int32_t velocityr;
int32_t accelerationr;

    OperationMode(od::positionmode);
    /* Converts from degrees/s to lines per ms, and then this value is shifted two bytes
     to the left to form the high part of the message. Then the low part is added.
     The high part corresponds to the increments/sample while the low part corresponds
     to the subdivion of an increment */
    velocityr=((velocity*Scaling_Factors_Velocity)*HIGHPART_BITSHIFT_16);

    // Motion profile type -  trapezoidal
    //WriteSDO(od::motion_profile_type,od::linear_ramp_trapezoidal);
    //Si paso los parametros convertidos en ui, sino convertir primero
    WriteSDO(od::profile_velocity,data32to4x8(velocityr));
    //Si paso los parametros convertidos en ui, sino convertir primero
    accelerationr=((acceleration*(Scaling_Factors_Acceleration))*HIGHPART_BITSHIFT_16);
    WriteSDO(od::profile_acceleration,data32to4x8(accelerationr));
//  The target position is the position that the drive should move to in
//  position profile mode using the current settings of motion control parameters
//  such as velocity, acceleration, and motion profile type etc.
//  It is given in position units.0x10000

//  Si paso los parametros convertidos en ui, sino convertir primero
    //WriteSDO(od::target_position,target);
    //WriteSDO(od::quick_stop_deceleration,deceleration);
//    FlushBuffer();
   return 0;
}

long CiA402Device::Setup_Velocity_Mode(const uint32_t acceleration, const uint32_t target){
//    In the Velocity Profile Mode the drive performs speed control.
//    The built-in reference generator computes a speed profile with a trapezoidal shape,
//    due to a limited acceleration. The Target Velocity object specifies
//    the jog speed (speed sign specifies the direction) and the Profile Acceleration
//    object the acceleration/deceleration rate.
cout<<"scaling "<< Scaling_Factors_Velocity<<endl;
    OperationMode(od::velocitymode);
    int32_t t=(target*Scaling_Factors_Velocity)*HIGHPART_BITSHIFT_16;
    int32_t accelerationr;

//    The target velocity is the input for the trajectory generator
//    and the value is given in user-defined velocity units.
//    Si paso los parametros convertidos en ui, sino convertir primero

    //targetr=( (target*(4096/360)*(1) ) << 16 )+0;//4096/360= [encoder-steps/deg] and 1000 [ms] in a [s]

     WriteSDO(od::target_velocity,data32to4x8(t));

    //accelerationr=((acceleration*4096/360000000))+0;
    accelerationr=acceleration*HIGHPART_BITSHIFT_16;
    WriteSDO(od::profile_acceleration,data32to4x8(accelerationr));
    return 0;
}




long CiA402Device::SetTarget_VELOCITY_PROPORCIONAL(double target, float kp){

    float consigna = 0; // Umbral de acercamiento a al traget en radianes
    float error;
    float threshold_target = 0.116;

    for(int i=0; i<2000; i++)
    {

        float pos = GetPosition();
        error = target - pos ;
        cout<<"  target- " <<target<<endl;



        if(fabs(error) < threshold_target)
            break;

        consigna = error * kp;

        cout<<"i= "<< i <<" | e =  "<< error << " |consigna = " << consigna << " | p = "<< pos*RADS2DEG << endl;

        SetVelocity(consigna);



        usleep(10000);

    }

    SetVelocity(0);


    return 0;
}


long CiA402Device::SetPositionRECURSIVE_test(long target){

    float threshold_target = 0.016; // Umbral de acercamiento a al traget en radianes
    SetPosition(target);
    for(int i=0; i<100; i++)
    {
        if(fabs(GetPosition()-target) < threshold_target)
            break;
        usleep(1000);
        SetPosition(target);
        cout<<" "<<i<<" "<< GetPosition()<<endl;
    }
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

long CiA402Device::SetPosition(double target){


    long retval;

    //Why is this needed?
    WritePDO1(od::goenable);

    //convert target to internal units value
    long target_t=(long)(target*Scaling_Factors_Position);

    //Write the IU target value in dictionary
    retval = WriteSDO(od::target_position,data32to4x8(target_t));
    //alternative, use PDO?
    //WritePDO4(data32to4x8(target));

    //lectura del status word y comprobar target reached (posicion bit11 = 1)
    //cada vez que la statusword cambia, envía un mensaje pdo.
    //revisar esto para esperar el mensaje correcto.
//    long stat = ReadSDO(od::statusword);
    //cout<<"statusword: "<<stat<<endl;
    //FlushBuffer();
    //sleep(1);

    //Send control word to update the system
    // ipos manual page 149
    //bits: 6=0(absolute) 5=1(immediatly) 4=1(update)
    // 0100000000111011 --> 40 3B
    // 0100000000111111 --> 40 3F
//    WritePDO1(data32to4x8(0x403B));
    //run via control word
    vector<uint8_t>cw={0x3F,0x00};
    WritePDO1(cw);
//    sleep(1);01100100
//    cout<<"RUN"<<endl;

//      WritePDO1(od::run);
     // FlushBuffer();
    return 0;//stat;
}

/// \brief CiA402Device::SetVelocity This function converts the velocity of a motor with an encoder of X lines in radians per second
/// into a variable of type uint32_t which contains the equivalent velocity as function of the enconder lines/sample.
/// \param target A uint32_t variable with the desired velocity in rad/s
/// \return This function re1turns a variable of type long with value 0.
///
long CiA402Device::SetVelocity(double target){


    int32_t targetr;

    double ui = target*Scaling_Factors_Velocity;

    if( ui < 1 && ui > 0)
        ui = HIGHPART_BITSHIFT_16;
    else if( ui > - 1 && ui < 0)
        ui = - HIGHPART_BITSHIFT_16;
    else if ( ui == 0)
        ui = 0;
    else
        ui = ui * HIGHPART_BITSHIFT_16;

    int32_t t = (int32_t)ui;
    //cout<<"ui- "<< target*Scaling_Factors_Velocity<< " t- " << t << " | target- " <<target<<endl;
    WriteSDO(od::target_velocity,data32to4x8(t));

    // Motor_Velocity[IU]=Scaling_factors_velocity*Load_Velocity[SI]
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

       OperationMode(od::torquemode);

    cout << "Setup_Torque_Mode " <<endl;
    WriteSDO(od::external_reference_type,od::torque_online_enable); /*received online via the CAN bus communication channel from the CANopen
                                                     master in object External On-line Reference (index 201C h )*/

    current_limit =  ReadSDO(od::current_limit);


     cout << "Current limit" <<  (current_limit) << endl;


    return 0;
}

long CiA402Device::SetTorque(double target){


    // Basado en el punto 16.2.1. Object 6071h: Target torque. La corriente en unidades internas es el
    // tanto por uno de la corriente de pico del "drive". Multiplicado por el rango de de entrada analogica,
    // dado que el torque lo seteamos de manera online.
    // formula: current [ IU ] =  65520 ⋅ current [ A ] /   2 ⋅ Ipeak
    // that in the targetr variable computation corresponds to:
    // 65520 = 0xFFF0 = ANALOGUE_INPUT_SCALE
    // current [ A ] / Ipeak = target = (peak current ratio received as input)
    //

    int32_t targetr= int32_t (target*ANALOGUE_INPUT_SCALE/2.0)*HIGHPART_BITSHIFT_16;
    if(target > 1)
        targetr = int32_t(ANALOGUE_INPUT_SCALE/2.0)*HIGHPART_BITSHIFT_16;
    else if(target < -1)
        targetr = int32_t(-ANALOGUE_INPUT_SCALE/2.0)*HIGHPART_BITSHIFT_16;

//    cout<<endl<<"Esto se envia a la funcion:"<<(long)target<<endl;
//    cout<<endl<<"Esto se envia de amperaje:"<<(int32_t)targetr<<endl;


    WriteSDO(od::external_reference, data32to4x8(targetr));

    //cout << " targetr: " << targetr << ", input target: " << target << ", " <<target*ANALOGUE_INPUT_SCALE/2.0 << endl;

    WritePDO1(od::run);
    //FlushBuffer();
    return 0;
}

long CiA402Device::SetAmpRaw(double target){
//    cout<<endl<<"Esto se envia a la funcion:"<<(int32_t)target<<endl;
    WriteSDO(od::external_reference, data32to4x8((int32_t)target));
    return 0;
    }

//long CiA402Device::SetTorque(double target){


//    int32_t targetr=(int32_t)target*MIN_SPEED_INC32;

//    if ((target < -1000))
//    {
//        targetr=-1000*MIN_SPEED_INC32;
//    }
//    if ((target > 1000))
//    {
//        targetr=1000*MIN_SPEED_INC32;
//    }
//    cout << " TORQUE : " << targetr << endl;

//    WriteSDO(od::torque_target,data32to4x8(targetr));

//    WritePDO(od::run);
//    FlushBuffer();
//    return 0;

//}

vector<uint8_t> data32to4x8(uint32_t in)
{

    vector<uint8_t> retvalue(4);
    retvalue[0] = in&0x000000FF;
    retvalue[1] = (in&0x0000FF00)>>8;
    retvalue[2] = (in&0x00FF0000)>>16;
    retvalue[3] = (in&0xFF000000)>>24;

    return retvalue;

}


vector<uint8_t> data16to2x8(uint16_t in)
{

    vector<uint8_t> retvalue(2);
    retvalue[0] = in&0x000000FF;
    retvalue[1] = (in&0x0000FF00)>>8;

    return retvalue;

}



/// Set the Nº lines for incremental encoder quadrature  (lines X 4)
long  CiA402Device::SetEnc_res(int lines)
{
    encoder_resolution = lines;
    return 0;
}

/// Set the transmission ratio between the motor displacement in SI units and load displacement
long  CiA402Device::SetRed_Mot(float reduction_ratio)
{
    reduction_ratio_motor = reduction_ratio;
    return 0;
}

/// Set the speed/position loop sampling period of the motor Control (sampling_slow_loop)
long  CiA402Device::SetSampling_period(float sampling_period)
{
    SampSL = sampling_period;
    return 0;
}

