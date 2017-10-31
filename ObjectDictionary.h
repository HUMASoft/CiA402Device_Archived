#ifndef OBJECTDICTIONARY_H
#define OBJECTDICTIONARY_H

#include <vector>
#include <stdint.h>

using namespace std;

namespace od
{
//REMEMBER TO USE CANBUS ENDIANNESS (lsb first)



//Values according to document CiA Draft Standard Proposal DSP-402 Version 1.1 (Date: October 8, 1998)
//const vector<uint8_t> controlword={0x40,0x60};
//const vector<uint8_t> statusword={0x41,0x60};
uint8_t acontrolword[]={0x40,0x60};
const vector<uint8_t> controlword(acontrolword,acontrolword+2);
uint8_t astatusword[]={0x41,0x60};
const vector<uint8_t> statusword(astatusword,astatusword+2);

}
#endif // OBJECTDICTIONARY_H
