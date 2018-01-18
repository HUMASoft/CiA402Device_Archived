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
#include "PortBase.h"


#define USE_TIMEOUT 200 //miliseconds timeout
#define FIND_RETRY 20


using namespace std;

class CiA301CommPort
{
public:
    CiA301CommPort(int newPortFileDescriptor, uint8_t new_id);
    CiA301CommPort(PortBase *new_port, uint8_t new_id);

    long ReadSDO(vector<uint8_t> address, int subindex);
    long ReadSDO(const vector<uint8_t> &address);
    long ReadNMT(const vector<uint8_t> &nmtCode);

    ///
    /// \brief WriteNMT This function sends NMT message from node to CanBus.
    /// \param nmtCommand The message that is sent.
    /// \return 0 if no error.
    ///
    long WriteNMT(const vector<uint8_t> &nmtCommand);

    ///
    /// \brief WritePDO This function sends PDO message to a node (the mode id is
    /// configured on constructor).
    /// \param command The mesage that is sent.
    /// \return 0 if no error.
    ///
    long WritePDO(const vector<uint8_t> &command);

    long FlushBuffer();
    long FlushBuffer(int msgs);
    long WriteSDO(const vector<uint8_t> &address, const vector<uint8_t> &value);
    long WritePDO4(const vector<uint8_t> &command);
    long ReadErrorNMT();
    long ReadPDO(long number);
private:

    //methods
    long CanOpenToCanBus(const co_msg &input, can_msg &output);
    long CanBusToCanOpen(const can_msg &input, co_msg &output);
    uint32_t data4x8to32(const uint8_t *in);
    uint32_t DataToInt(const uint8_t *in, const uint8_t size);

    int SendMessage(co_msg input);
    int SendCanMessage(can_msg &input);

    int WaitForReadMessage(co_msg &output, unsigned int canIndex);

    int ReadCobId(uint16_t expected_cobid, co_msg &output);
    int read_timeout(int fd, can_msg *buf, unsigned int timeout);
    long GetMsg(can_msg &msg);



    //variables
    int portFileDescriptor;
    uint8_t id;
    PortBase* port;
    bool usesockets;

    can_msg send_msg;
    can_msg input;
    vector<can_msg> readBuffer;
    vector<unsigned int> readInbox;

    co_msg SetCanOpenMsg(unsigned short id_co, unsigned short rtr,  vector<uint8_t> coDataFrame);
};


namespace sdo
{

const uint16_t tx0=0x580;
const uint16_t rx0=0x600;


}
namespace pdo
{

const uint16_t tx0=0x180;
const uint16_t rx0=0x200;
const uint16_t tx1=0x280;
const uint16_t rx1=0x300;
const uint16_t tx4=0x380;
const uint16_t rx4=0x400;

}
namespace nmt
{
const vector<uint8_t> started={0x01};

}

#endif // CIA301COMMPORT_H
