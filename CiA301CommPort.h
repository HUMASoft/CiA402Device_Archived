#ifndef CIA301COMMPORT_H
#define CIA301COMMPORT_H


#include <iostream>
#include <bitset>
#include <err.h>
#include <unistd.h>
#include <vector>
#include <string.h>


#include "candatatypes.h"




using namespace std;

class CiA301CommPort
{
public:
    CiA301CommPort(int newPortFileDescriptor);

    long ReadSDO(int index, int subindex, int id);
    long ReadSDO(vector<uint8_t> address, int id);

private:

    long CanOpenToCanBus(const co_msg &input, can_msg &output);
    int SendMessage(co_msg input, unsigned int canIndex);

    int portFileDescriptor;

    can_msg send_msg;

    co_msg SetCanOpenMsg(unsigned short id_co, unsigned short rtr,  vector<uint8_t> coDataFrame);
};

#endif // CIA301COMMPORT_H
