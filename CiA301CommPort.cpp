#include "CiA301CommPort.h"


CiA301CommPort::CiA301CommPort(int newPortFileDescriptor, uint8_t new_id)
{

    portFileDescriptor = newPortFileDescriptor;

    id=new_id;


}

long CiA301CommPort::ReadSDO(const vector<uint8_t> &address)
{

    co_msg output;

    //cout << "id" << id << endl;
    //Ask an sdo read from address
    SendMessage(SetCanOpenMsg(sdo::rx0+id, 0 ,address) );

    //Wait for the answer
    output = SetCanOpenMsg(sdo::tx0+id, 0 ,address);
    ReadCobId(sdo::tx0+id,output);

    //Get the data from output


    return output.data_co[4]+output.data_co[5];
}

long CiA301CommPort::WritePDO(const vector<uint8_t> &command)
{

    //co_msg output;

    //cout << "id" << id << endl;
    //Ask an sdo read from address
    SendMessage(SetCanOpenMsg(pdo::rx0+id, 0 ,command) );

    //Wait for the answer
    //output = SetCanOpenMsg(sdo::tx0+id, 0 ,address);


    //Get the data from output
    //ReadCobId(pdo::tx0+id,output);

    return 0;
}

long CiA301CommPort::FlushBuffer()
{
    cout << "Flushing can buffer" << endl;
    co_msg m1;
    while (WaitForReadMessage(m1,0)==0)
    {

    }
    return 0;
}

long CiA301CommPort::WriteNMT(const vector<uint8_t> &nmtCommand)
{

    co_msg output;
    vector<uint8_t> data(nmtCommand);
    data.push_back(id);
    //cout << "id" << id << endl;
    //Ask an sdo read from address
    SendMessage(SetCanOpenMsg(0, 0 ,data) );

    //Wait for the answer
    //output = SetCanOpenMsg(sdo::tx0+id, 0 ,address);


    //Get the data from output


    return ReadCobId(0x700+id,output);
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


/* Función para convertir de can a canopen */
long CiA301CommPort::CanBusToCanOpen(const can_msg & input, co_msg & output)
{

   //verificar el bit de start to frame de can

    output.dlc_co=input.dlc;
    output.id_co=input.id;
    output.rtr=input.rtr;
    output.ts=input.ts;


    for( int i=0; i < 8; i++)
    {
        output.data_co[i] = input.data[i];
    }

    return 0;
}



/**
 * @brief CiA402DeviceICanbus::SetCanOpenMsg : Constructs canopen message from parameters
 * @param id_co: cob id canopen parameter.
 * @param rtr: request for remote.
 * @param msg_start : canopen data frame.
 * @return : canopen constructed message in co_msg data type.
 */
co_msg CiA301CommPort::SetCanOpenMsg(unsigned short id_co, unsigned short rtr, vector<uint8_t> coDataFrame){

    co_msg msg_co;
    msg_co.id_co=id_co;
    msg_co.dlc_co=coDataFrame.size();

    memcpy(msg_co.data_co, coDataFrame.data(), (msg_co.dlc_co)*sizeof(uint8_t));
    //msg_co.nodeID=nodeID;
    msg_co.rtr=rtr;
    //msg_co.fun_code=fund_code;
//    cout<<"cob id: " << std::hex << msg_co.id_co << std::dec << " rtr: " << msg_co.rtr << endl;

//    cout<<"data: ";
//    for (int i = 0; i < msg_co.dlc_co; i++) {

//        printf("%02x ",msg_co.data_co[i]);
//    }
//    cout<<endl;
    return msg_co;
}



/* Transforma mensaje de canopen a can y lo envía al puerto  */
int CiA301CommPort::SendMessage(co_msg input)
{
    cout<<"SendMessage " << endl;
    cout<<"sent cob id " << std::hex << input.id_co << std::dec <<" rtr: " << input.rtr << endl;

    if (CanOpenToCanBus(input,send_msg) < 0)
    {
        cout<<"Error al transformar el mensaje"<<endl;
    }

    else
    {
        //print can frame information
        cout<<"sent can id " << (bitset<16>)send_msg.id << " rtr: " << send_msg.rtr << endl;

        cout<<"sent data: ";
        for (int i = 0; i < send_msg.dlc; i++)
        {

            printf("%02x ",send_msg.data[i]);
        }
        cout<<endl;

        /* Write the message to the Port */
        if(write(portFileDescriptor,&send_msg,sizeof(struct can_msg))!=sizeof(struct can_msg))
        {
            err(1, "Failed to send message");

        }

    }
    return 0;
}


int CiA301CommPort::WaitForReadMessage(co_msg & output, unsigned int canIndex){

    can_msg input;
    cout<<"WaitForReadMessage " << endl;

#if USE_TIMEOUT
    if(read_timeout(portFileDescriptor,&input,USE_TIMEOUT)==0)
    {
        err(1,"Could not read the message.");
        return -1;
    }

#else //(NOT)USE_TIMEOUT
    if( read(portFileDescriptor,&input,sizeof(struct can_msg) !=sizeof(input)) )
    {
        err(1,"read");
    }
#endif //USE_TIMEOUT


    //convert received data to canopen
    if(CanBusToCanOpen(input, output)!=0)
    {
       err(1,"error al convertir el mensaje");
       return -1;
    }
    else
    {
        //print can frame information
        cout<<"received can id " << (bitset<16>)input.id << " rtr: " << input.rtr << endl;
        cout<<"received data: ";
        for (int i = 0; i < input.dlc; i++)
        {

            printf("%02x ",input.data[i]);
        }
        cout<<endl;
                cout<<"received cob id " << std::hex << output.id_co << std::dec << " rtr: " << output.rtr << endl;
                cout<<"received canopen data: ";
                for (int i = 0; i < output.dlc_co; i++)
                {

                    printf("%02x ",output.data_co[i]);
                }
                cout<<endl;
    }
    return 0;
}

///
/// \brief CiA301CommPort::WaitForAnswer Read the port until expected canopen answer shows
/// up in the port reads.
/// \param output Expected canopen answer.
/// \param canIndex: Index of the caller.
/// \return
///
int CiA301CommPort::ReadCobId(uint16_t expected_cobid, co_msg & output ){

    cout<<"ReadCobId " << endl;


#if USE_TIMEOUT
    if(read_timeout(portFileDescriptor,&input,USE_TIMEOUT)==0)
    {
        err(1,"Could not read the message.");
        return -1;
    }

#else //(NOT)USE_TIMEOUT
    if( read(portFileDescriptor,&input,sizeof(struct can_msg) !=sizeof(input)) )
    {
        err(1,"read");
    }
#endif //USE_TIMEOUT

    //First check for the output.
    //If the response is the answer expected, continue
    if (input.id != expected_cobid)//Otherwise, store in a buffer to avoid loose data and read again
    {

        //check node id
        if (GET_CO_ID(input.id) != GET_CO_ID(expected_cobid) )
        {

            cout << "Cobid not received. Error: " << input.id << endl;
return -1;
        }
        //if id, check if error

        //oterwise, resend to canopen to avoid losing messages


    }

    //convert received data to canopen
    if(CanBusToCanOpen(input, output)!=0)
    {
       err(1,"error al convertir el mensaje");
       return -1;
    }
    else
    {
        //print can frame information
        cout<<"received can id " << (bitset<16>)input.id << " rtr: " << input.rtr << endl;
        cout<<"received data: ";
        for (int i = 0; i < input.dlc; i++)
        {

            printf("%02x ",input.data[i]);
        }
        cout<<endl;
                cout<<"received cob id " << std::hex << output.id_co << std::dec <<" rtr: " << output.rtr << endl;
                cout << "ID: " << GET_CO_ID(output.id_co) << endl;
                cout<<"received canopen data: ";
                for (int i = 0; i < output.dlc_co; i++)
                {

                    printf("%02x ",output.data_co[i]);
                }
                cout<<endl;
    }

    return 0;
}

int CiA301CommPort::read_timeout(int fd, struct can_msg *buf, unsigned int timeout)
{
    struct timeval tv;
    int sec,ret;

    sec=timeout/1000;
    tv.tv_sec=sec;
    tv.tv_usec=(timeout-(sec*1000))*1000;

    //use select to check readyness
    fd_set fds; // file descriptor set
    FD_ZERO(&fds);// clear the set
    FD_SET(fd,&fds);// put the fd in the set
    assert(FD_ISSET(fd,&fds));
    ret=select(fd+1,&fds,0,0,&tv);
    //cout<< "Number of ready ports: " << ret <<endl;

    if(ret==0)/* timeout */
    {
        err(1,"Port connection timed out");
        return 0;

    }
    else if (ret<0)
    {
        err(ret,"Numbered Error. See exit code");
        return ret;

    }
    else
    {
        //TODO *** Check if pread works with canbus ports at 0 offset
        return pread(fd,buf,sizeof(struct can_msg),0);
    }

}

