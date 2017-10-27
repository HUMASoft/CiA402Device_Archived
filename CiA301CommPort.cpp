#include "CiA301CommPort.h"


CiA301CommPort::CiA301CommPort(int newPortFileDescriptor)
{

    portFileDescriptor = newPortFileDescriptor;


}

long CiA301CommPort::ReadSDO(vector<uint8_t> address, int id)
{


    SendMessage(SetCanOpenMsg(0x600+id, 0 ,address) , id);

    return 0;
}


long CiA301CommPort::CanOpenToCanBus(const co_msg & input, can_msg & output)
        {

                output.id=input.id_co;


                //output.fi=input.id_co;
                //output.fi<<=4;
                //output.fi+=input.dlc_co;
                output.dlc=input.dlc_co;
                output.rtr=input.rtr;

                output.ff=0; //normal frame
                output.ts=input.ts;
                // creo que en vez de 8 poner dlc
                for( int i=0; i < 8; i++)
        {
                output.data[i] = input.data_co[i];
}



                return 0;
}

co_msg CiA301CommPort::SetCanOpenMsg(unsigned short id_co, unsigned short rtr, vector<uint8_t> coDataFrame){

    co_msg msg_co;
    msg_co.id_co=id_co;
    msg_co.dlc_co=coDataFrame.size();

    memcpy(msg_co.data_co, coDataFrame.data(), (msg_co.dlc_co)*sizeof(uint8_t));
    //msg_co.nodeID=nodeID;
    msg_co.rtr=rtr;
    //msg_co.fun_code=fund_code;
    cout<<"cob id " << (bitset<16>)msg_co.id_co << " rtr: " << msg_co.rtr << endl;

    cout<<"data: ";
    for (int i = 0; i < msg_co.dlc_co; i++) {

        printf("%02x ",msg_co.data_co[i]);
    }
    cout<<endl;
    return msg_co;
}



/* Transforma mensaje de canopen a can y lo envía al puerto  */
int CiA301CommPort::SendMessage(co_msg input, unsigned int canIndex)
{

    if (CanOpenToCanBus(input,send_msg) < 0){
        cout<<"Error al transformar el mensaje"<<endl;
    }

    else{
        //print can frame information
        cout<<"sent can id " << (bitset<16>)send_msg.id << " rtr: " << send_msg.rtr << endl;
        cout<<"sent data: ";
        for (int i = 0; i < send_msg.dlc; i++) {

            printf("%02x ",send_msg.data[i]);
        }
        cout<<endl;

        /* Write the message to the Can0 */
        if(write(portFileDescriptor,&send_msg,sizeof(struct can_msg))!=sizeof(send_msg)){
        err(1, "Failed to send message");

        }

    }
    return 0;
}
