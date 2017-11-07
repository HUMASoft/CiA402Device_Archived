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


const vector<uint8_t> checkerror={0x02,0x10};


}
#endif // OBJECTDICTIONARY_H
