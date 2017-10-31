#ifndef CIA301COMMPORT_H
#define CIA301COMMPORT_H


#include <iostream>
#include <bitset>
#include <err.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>


#include "candatatypes.h"




using namespace std;

class CiA301CommPort
{
public:
    CiA301CommPort(int newPortFileDescriptor);

    long ReadSDO(vector<uint8_t> address, int subindex, int id);
    long ReadSDO(vector<uint8_t> address, int id);

private:

    //methods
    long CanOpenToCanBus(const co_msg &input, can_msg &output);
    long CanBusToCanOpen(const can_msg &input, co_msg &output);


    int SendMessage(co_msg input, unsigned int canIndex);
    int WaitForReadMessage(co_msg &output, unsigned int canIndex);
    int WaitForAnswer(co_msg &output, unsigned int canIndex);
    int read_timeout(int fd, can_msg *buf, unsigned int timeout);


    //variables
    int portFileDescriptor;

    can_msg send_msg;

    co_msg SetCanOpenMsg(unsigned short id_co, unsigned short rtr,  vector<uint8_t> coDataFrame);
};


namespace cia301
{

const uint16_t tx=0x580;
const uint16_t rx=0x600;


}


#endif // CIA301COMMPORT_H
