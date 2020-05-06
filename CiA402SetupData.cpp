#include "CiA402SetupData.h"

CiA402SetupData::CiA402SetupData()
{

}

CiA402SetupData::CiA402SetupData(int new_encRes,float new_mlRatio, float new_SampSL,float motor_current_limit,float drive_current_limit)
{

    mlRatio = new_mlRatio;// Transmission ratio between the motor displacement in SI units and load displacement
    encRes = new_encRes;// Nº pulses for incremental encoder quadrature  (lines X 4)
    SampSL = new_SampSL;// speed/position loop sampling period of the motor Control (sampling_slow_loop)
    current_limit = 32767-(65520*motor_current_limit)/(2*drive_current_limit);// current_limit. CAnOpen programming iPOS 5.5.7. Object 207Fh: Current limit


    float count_sec = encRes/60.0;
    float rad2rpm = 60.0/(2.0*M_PI);


    // Motor_Position[IU]=Scaling_factors_velocity*Load_Position[SI]
    Scaling_Factors_Position = mlRatio*encRes/(2.0*M_PI);

    // Motor_Speed[IU]=Scaling_factors_velocity*Load_Speed[SI]
    Scaling_Factors_Velocity = rad2rpm*count_sec*mlRatio*SampSL;
    cout << "Scaling_Factors_Velocity - " << Scaling_Factors_Velocity << endl;

    // Motor_Accel[IU]=Scaling_factors_velocity*Load_Accel[SI]
    Scaling_Factors_Acceleration = Scaling_Factors_Velocity*SampSL;

}

float CiA402SetupData::getScaling_Factors_Velocity() const
{
    return Scaling_Factors_Velocity;
}

float CiA402SetupData::getScaling_Factors_Position() const
{
    return Scaling_Factors_Position;
}

float CiA402SetupData::getScaling_Factors_Acceleration() const
{
    return Scaling_Factors_Acceleration;
}

int CiA402SetupData::getEncRes() const
{
    return encRes;
}

