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
    ulong ReadSDO(const vector<uint8_t> &address);
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
    long WritePDO(long number, const vector<uint8_t> &command);

    long FlushBuffer();

    ///
    /// \brief FlushBuffer Removes the msgs number of can frames from buffer
    /// \param msgs Number of messages to remove.
    /// \return 0 if no error.
    ///
    long FlushBuffer(int msgs);

    ///
    /// \brief WriteSDO Writes 4 byte value to specific address on device.
    /// This function sends SDO message requesting write on the object dictionary
    /// address. Also block and wait for the SDO msg for write ack, and returns negative if error.
    /// As it reads the ack SDO message, the buffer should be same size after this function
    /// returns.
    /// \param address The address that needs SDO writing.
    /// \param value The value that will be writed.
    /// \return 0 if no error. Negative if error (see cerr).
    ///
    long WriteSDO(const vector<uint8_t> &address, const vector<uint8_t> &value);
    long WritePDO4(const vector<uint8_t> &command);
    long ReadErrorNMT();
    long ReadPDO(long number);
    long DisablePDOs();
    long EnablePDOs();


    uint32_t data4x8to32(const uint8_t *in, int dlc);
    //co_msg SetCanOpenMsg(unsigned short id_co, unsigned short rtr, vector<uint8_t> coDataFrame, int size);



    long WritePDO1(const vector<uint8_t> &command);
protected:

    //variables
    int portFileDescriptor;
    uint8_t id;
    PortBase* port;
    bool usesockets;

private:

    //methods

    ///
    /// \brief CanOpenToCanBus: Converts a CanOpen object or message into a CanBus object or message.
    ///For this, it receives two parameters passed by reference, one of an object
    ///CanOpen that will become the other CanBus object. To do this, copy the ids,
    ///the dlc, the rtr, the ts, select the normal frame for the CanBus message and
    ///finally copy the message information.
    /// \param input CanOpen message.
    /// \param output Can message.
    /// \return
    ///
    long CanOpenToCanBus(const co_msg &input, can_msg &output);

    ///
    /// \brief CanBusToCanOpen: It performs the inverse task to the previous function. This function converts a
    ///CanBus object or message in another CanOpen object or message both passed
    ///by reference. To do this, copy the information in the CanOpen message
    ///CanBus message data: the id, the dlc, the rtr, the ts and the data information
    ///which contains (the content of the message itself).
    /// \param input Can message.
    /// \param output CanOpen message.
    /// \return
    ///
    long CanBusToCanOpen(const can_msg &input, co_msg &output);
    // uint32_t data4x8to32(const uint8_t *in, int dlc);
    ulong DataToInt(const uint8_t *in, unsigned short size);

    int SendMessage(co_msg input);
    int SendCanMessage(can_msg &input);

    int WaitForReadMessage(co_msg &output, unsigned int canIndex);

    int ReadCobId(uint16_t expected_cobid, co_msg &output);
    int read_timeout(int fd, can_msg *buf, unsigned int timeout);
    long GetMsg(can_msg &msg);





    can_msg send_msg;
    can_msg input;
    vector<can_msg> readBuffer;
    vector<unsigned int> readInbox;

    /**
     * @brief CiA301CommPort::SetCanOpenMsg : Constructs canopen message from parameters
     * @param id_co: cob id canopen parameter.
     * @param rtr: request for remote.
     * @param msg_start : canopen data frame.
     * @return : canopen constructed message in co_msg data type.
     */

    co_msg SetCanOpenMsg(unsigned short id_co, unsigned short rtr, vector<uint8_t> coDataFrame);};


namespace sdo
{

const uint16_t tx0=0x580;
const uint16_t rx0=0x600;


}
namespace pdo
{

const vector<uint8_t> tx0enable={0x00,0x1A,0x00};//TPDO1
const vector<uint8_t> tx0sub1={0x00,0x1A,0x01};//TPDO1.1
const vector<uint8_t> tx0sub2={0x00,0x1A,0x02};//TPDO1.2
const vector<uint8_t> tx1enable={0x01,0x1A,0x00};//TPDO2
const vector<uint8_t> tx2enable={0x02,0x1A,0x00};//TPDO3
const vector<uint8_t> tx3enable={0x03,0x1A,0x00};//TPDO4


const uint16_t tx0=0x180;//TPDO1
const uint16_t rx0=0x200;//TPDO1
const uint16_t tx1=0x280;//TPDO2
const uint16_t rx1=0x300;//TPDO2
const uint16_t tx2=0x380;//TPDO3
const uint16_t rx2=0x400;//TPDO3
const uint16_t tx3=0x480;//TPDO4
const uint16_t rx3=0x500;//TPDO4

}
namespace nmt
{
const vector<uint8_t> started={0x01};

}

#endif // CIA301COMMPORT_H
