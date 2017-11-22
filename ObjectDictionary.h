#ifndef OBJECTDICTIONARY_H
#define OBJECTDICTIONARY_H

#include <vector>
#include <stdint.h>

using namespace std;

namespace od
{
//REMEMBER TO USE CANBUS ENDIANNESS (lsb first)

//Values according to document CiA Draft Standard Proposal DSP-402 Version 1.1 (Date: October 8, 1998)
const vector<uint8_t> controlword={0x40,0x60};
const vector<uint8_t> statusword={0x41,0x60};
const vector<uint8_t> reset={0x81};
const vector<uint8_t> start={0x01};
const vector<uint8_t> readytoswitchon={0x06,0x00};
const vector<uint8_t> switchon={0x07,0x00};
const vector<uint8_t> enable={0x0F,0x00};
const vector<uint8_t> run={0x1F,0x00};
const vector<uint8_t> quickstop={0x02,0x00};//{0x04,0x00};
const vector<uint8_t> OperationMode={0x60,0x60,0x00};
//move to other header
const vector<uint8_t> positionmode={0x01};
const vector<uint8_t> velocitymode={0x03};
const vector<uint8_t> quick_stop_mode={0x5A,0x60};
const vector<uint8_t> stop_option_code={0x5D,0x60};

const vector<uint8_t> checkerror={0x02,0x10};

const vector<uint8_t> positionaddress={0x63,0x60}; //check this!!!
const vector<uint8_t> velocityaddress={0x6C,0x60};
const vector<uint8_t> target_position={0x7A,0x60};
const vector<uint8_t> profile_acceleration={0x83,0x60};
const vector<uint8_t> quick_stop_deceleration={0x85,0x60};
const vector<uint8_t> motion_profile_type={0x86,0x60};
const vector<uint8_t> profile_velocity={0x81,0x60};
const vector<uint8_t> linear_ramp_trapezoidal={0x00};
const vector<uint8_t> target_velocity={0xFF,0x60};

}
#endif // OBJECTDICTIONARY_H
