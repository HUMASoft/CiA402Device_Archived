#ifndef OBJECTDICTIONARY_H
#define OBJECTDICTIONARY_H

namespace od
{
//REMEMBER TO USE CANBUS ENDIANNESS (lsb first)

//Values according to document CiA Draft Standard Proposal DSP-402 Version 1.1 (Date: October 8, 1998)
const unsigned char controlword[2]={0x40,0x60};
const unsigned char statusword[2]={0x41,0x60};
}
#endif // OBJECTDICTIONARY_H
