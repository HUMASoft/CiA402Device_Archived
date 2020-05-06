#ifndef CIA402SETUPDATA_H
#define CIA402SETUPDATA_H


#include <math.h>
#include <iostream>



using namespace std;

class CiA402SetupData
{
public:
    CiA402SetupData();

    CiA402SetupData(int new_encRes, float new_mlRatio, float new_SampSL, float motor_current_limit, float drive_current_limit);
    float getScaling_Factors_Velocity() const;

    float getScaling_Factors_Position() const;

    float getScaling_Factors_Acceleration() const;

    int getEncRes() const;

private:



    float mlRatio;    // Transmission ratio between the motor displacement in SI units and load displacement
    int encRes;         // Nº lines for incremental encoder quadrature  (lines X 4)
    float SampSL;                   // speed/position loop sampling period of the motor Control (sampling_slow_loop)
    int current_limit;              // current_limit. CAnOpen programming iPOS 5.6.7. Object 207Fh: Current limit

   //Technosoft drives work with parameters and variables represented in the drive internal units (IU).
   // Constant to convert velocity from rad to count/sample
   // The internal speed units are internal position units/(slow loop sampling period)
    float Scaling_Factors_Velocity;
    float Scaling_Factors_Position;
    float Scaling_Factors_Acceleration;

};

#endif // CIA402SETUPDATA_H
