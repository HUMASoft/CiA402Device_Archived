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
const vector<uint8_t> commreset={0x81};
const vector<uint8_t> fullreset={0x82};
const vector<uint8_t> start={0x01};
const vector<uint8_t> goreadytoswitchon={0x06,0x00};
const vector<uint8_t> goswitchon={0x07,0x00};
const vector<uint8_t> goenable={0x0F,0x00};
const vector<uint8_t> goswitchondisable={0x00,0x00};
const vector<uint8_t> run={0x1F,0x00};
const vector<uint8_t> expedite={0x3F,0x00}; //like run, but dont finish actual position profile

const vector<uint8_t> quickstop={0x02,0x00};//{0x04,0x00};
const vector<uint8_t> OperationMode={0x60,0x60,0x00};
const vector<uint8_t> OperationModeDisplay={0x61,0x60,0x00};

//move to other header
const vector<uint8_t> positionmode={0x01};
const vector<uint8_t> velocitymode={0x03};
const vector<uint8_t> quick_stop_mode={0x5A,0x60};
const vector<uint8_t> stop_option_code={0x5D,0x60};

const vector<uint8_t> checkerror={0x02,0x10};

//address must be complete index and subindex {I1,I2,S1}
const vector<uint8_t> positionaddress={0x63,0x60};
const vector<uint8_t> velocityactvalue={0x69,0x60};
const vector<uint8_t> velocityaddress={0x69,0x60};
const vector<uint8_t> target_position={0x7A,0x60,0x00};
const vector<uint8_t> position_demand={0x62,0x60,0x00};

const vector<uint8_t> profile_acceleration={0x83,0x60,0x00};
const vector<uint8_t> quick_stop_deceleration={0x85,0x60,0x00};
const vector<uint8_t> motion_profile_type={0x86,0x60};
const vector<uint8_t> profile_velocity={0x81,0x60,0x00};
const vector<uint8_t> linear_ramp_trapezoidal={0x00};
const vector<uint8_t> target_velocity={0xFF,0x60};
const vector<uint8_t> velocity_encoder_resolution_num={0x94,0x60,0x01};
const vector<uint8_t> velocity_encoder_resolution_den={0x94,0x60,0x02};

const vector<uint8_t> gear_ratio={0x91,0x60,0x00};
}
#endif // OBJECTDICTIONARY_H
