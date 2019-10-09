#include "CiA301CommPort.h"


CiA301CommPort::CiA301CommPort(int newPortFileDescriptor, uint8_t new_id)
{

    portFileDescriptor = newPortFileDescriptor;
    usesockets=0;

    id=new_id;


}

CiA301CommPort::CiA301CommPort(PortBase* new_port, uint8_t new_id)
{

    port=new_port;
    usesockets=1;

    id=new_id;

    //set the filter to port so commport will only accept messages to his id
    //must decide to make not constant. Till then, moved up to main
    port->SetFilter(id,0x07F);
    //port->SetFilter(0x000,0x7FF);

}
///
/// \brief CiA301CommPort::ReadSDO Waits until expected
/// \param can SDO address
/// \return
///
ulong CiA301CommPort::ReadSDO(const vector<uint8_t> &address)
{

    co_msg output;

    //cout << "id" << id << endl;
    //Ask an sdo read from address
    vector<uint8_t> data={};
//   data.insert(data.end(), address.begin(), address.end());
//    SendMessage(SetCanOpenMsg(sdo::rx0+id, 0 ,data) );

    //Reading is got from an empty write response
    ulong ret=(ulong)WriteSDO(address, data);
   // cout << "ReadSDO ret: " << ret << endl;
     return ret;

//    //Wait for the answer
//    output = SetCanOpenMsg(sdo::tx0+id, 0 ,address);
//    ReadCobId(sdo::tx0+id,output);


    //Get the data from output
    //Use the data from output
    //TODO: control response with read table (table 8 man 301)
//    if (output.data_co[0]!=0x60)
//    {
//        err(1,"Can not receive answer from node");
//    }

//fixed return four last bytes from data.
//    long retvalue = output.data_co[7];
//    retvalue = (retvalue << 8) + output.data_co[6];
//    retvalue = (retvalue << 8) + output.data_co[5];
//    retvalue = (retvalue << 8) + output.data_co[4];

//    return output.data_co[4]+(16^2)*output.data_co[5]+
//            (16^4)*output.data_co[6]+(16^6)*output.data_co[7];
//    return retvalue;
}

long CiA301CommPort::WriteSDO(const vector<uint8_t> &address, const vector<uint8_t> &value )
{


    //Set the first byte of data frame
    //it holds the size descriptor for the data bytes
    //like in CiA 301

    vector<uint8_t> data(1);
    switch(value.size()){
    case 0 :

        data[0]=0x40;
        break;
    case 1 :

        data[0]=0x2F;
        break;
     case 2 :

        data[0]=0x2B;
        break;
    default:

       data[0]=0x23;
       break;

    }

   //Insert SDO Address
   data.insert(data.end(), address.begin(), address.end());

   //Insert SDO data field
   data.insert(data.end(), value.begin(), value.end());

    //cout << "id" << id << endl;
    //Write the data in the default rx port
    //like in CiA 301

    int size=value.size();
    SendMessage( SetCanOpenMsg(sdo::rx0+id, 0 ,data) );

//    //and wait for write response
//    co_msg output;

    //every write operation returns an empty answer if correct
    //wait the answer in TPDO1 (tx0(580)+id)
    co_msg output;
    ReadCobId(sdo::tx0+id, output);
    //alternative: use PDO, but no check is available.
//    if (ReadPDO(0) != 0) return -1;



    for (long reps=0 ; reps<FIND_RETRY ;reps++)
    {

        //cout << output.data_co[1]<< " " << address[0]<< " " << output.data_co[2]<< " " << address[1] <<endl;
        //Check that received data comes from right address.
        if ( (output.data_co[1] == address[0]) && (output.data_co[2] == address[1]) )
        {

            //cout << "break";
            break;
        }

        else //Wrong address.
        {

            //TODO: Return message to bus??
            //SendMessage(output);
            //Read again and check (later)
            ReadCobId(sdo::tx0+id,output);

        }



    }

    //fixed return four last bytes from data.
//    cout<<"OUTPUT DATA 0    "<<(int)output.data_co[0]<<endl;
//    cout<<"OUTPUT DATA 1    "<<(int)output.data_co[1]<<endl;
//    cout<<"OUTPUT DATA 2    "<<(int)output.data_co[2]<<endl;
//    cout<<"OUTPUT DATA 3    "<<(int)output.data_co[3]<<endl;
//    cout<<"OUTPUT DATA 4    "<<(bitset<8>)output.data_co[4]<<endl;
//    cout<<"OUTPUT DATA 5    "<<(bitset<8>)output.data_co[5]<<endl;
//    cout<<"OUTPUT DATA 6    "<<(bitset<8>)output.data_co[6]<<endl;
//    cout<<"OUTPUT DATA 7    "<<(bitset<8>)output.data_co[7]<<endl;
//    long retvalue = output.data_co[7];
//    retvalue = (retvalue << 8) + output.data_co[6];
//    retvalue = (retvalue << 8) + output.data_co[5];
//    retvalue = (retvalue << 8) + output.data_co[4];
//    return retvalue;
//    cout << "dlc " << output.dlc_co << endl;
//    long retvalue=DataToInt(&output.data_co[4], output.dlc_co);

    long ret=DataToInt(&output.data_co[4], output.dlc_co-4);
    //cout << "WriteSDO ret: " << ret << endl;
    return ret;
}


///
/// \brief CiA301CommPort::ReadPDO
/// \param number: Use numbers between 0(TPDO1) and 3 (TPDO4).
/// \return Data field response converted to long.
///
long CiA301CommPort::ReadPDO(long number)
{

    co_msg output;

    ReadCobId((number*0x100)+pdo::tx0+id,output);

    //TODO: check response
//    for (long reps=0 ; reps>FIND_RETRY ;reps++)
//    {
//        //Check that received data comes from right address.
//        if ( (output.data_co[1] == address[0]) && (output.data_co[2] == address[1]) )
//        {
//            continue;
//        }

//        else //Wrong address.
//        {
//            cout << output.data_co[1]<< " " << address[0]<< " " << output.data_co[2]<< " " << address[1] <<endl;

//            //TODO: Return message to bus??
//            //SendMessage(output);
//            //Read again and check (later)
//            ReadCobId(sdo::tx0+id,output);

//        }



//    }

    //Get the data from output
    //Use the data from output
    //TODO: control response with read table (table 8 man 301)
//    if (output.data_co[0]!=0x60)
//    {
//        err(1,"Can not receive answer from node");
//    }

    return DataToInt(output.data_co,output.dlc_co);
}

long CiA301CommPort::DisablePDOs()
{
    //Can Open iPos manual page 194
    //Send the following message:
    //(SDO write access to object 1A00 h sub-index 0 the 8-bit value 00 h ):
    vector<uint8_t> data={0x00};
    cout << WriteSDO(pdo::tx0enable ,data);

    //Can Open iPos manual page 268
    //Send the following message:
    //(SDO write access to object 1A01 h sub-index 0 the 8-bit value 00 h ):
    cout << WriteSDO(pdo::tx1enable ,data);
    cout << WriteSDO(pdo::tx2enable ,data);
    cout << WriteSDO(pdo::tx3enable ,data);

    return 0;
}

long CiA301CommPort::EnablePDOs()
{
    //Can Open iPos manual page 194
    //Send the following message:
    //(SDO write access to object 1A00 h sub-index 0 the 8-bit value 00 h ):
    vector<uint8_t> data={0x10, 0x00, 0x41, 0x60};
    cout << WriteSDO(pdo::tx0sub1 ,data);
    data={0x02};
    cout << WriteSDO(pdo::tx0enable ,data);

    //Can Open iPos manual page 268
    //Send the following message:
    //(SDO write access to object 1A01 h sub-index 0 the 8-bit value 00 h ):
    cout << WriteSDO(pdo::tx1enable ,data);
    cout << WriteSDO(pdo::tx2enable ,data);
    cout << WriteSDO(pdo::tx3enable ,data);

    return 0;
}

long CiA301CommPort::WritePDO( long number,const vector<uint8_t> &command)
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



long CiA301CommPort::WritePDO1(const vector<uint8_t> &command)
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

long CiA301CommPort::WritePDO4(const vector<uint8_t> &command)
{

    //co_msg output;

    //cout << "id" << id << endl;
    //Ask an sdo read from address
    SendMessage(SetCanOpenMsg(pdo::rx3+id, 0 ,command) );

    //Wait for the answer
    //output = SetCanOpenMsg(sdo::tx0+id, 0 ,address);


    //Get the data from output
    //ReadCobId(pdo::tx0+id,output);

    return 0;
}

long CiA301CommPort::FlushBuffer()
{
//    cout << "Flushing can buffer" << endl;
    co_msg m1;


    port->FlushMsg();
    //maybe flush nmt will be needed sometime

//    if (usesockets)
//    {
//        cout << "Cant Flush can buffer. Using blocking read!!!!" << endl;
//        return 1;
////        for (int i=0; i<5; i++)
////        {
////            uint32_t id; uint8_t data[8];int size;
////            port->GetMsg(id,data,size);
////        }
//    }

//    while (WaitForReadMessage(m1,0)==0)
//    {

//    }
    return 0;
}

long CiA301CommPort::FlushBuffer(int msgs)
{

    for (int i=0; i<msgs; i++)
    {

        GetMsg(input);
        cout << "cleared : " << hex << input.id << dec << endl;

    }
    return 0;
}


long CiA301CommPort::WriteNMT(const vector<uint8_t> &nmtCommand)
{


    co_msg output;
    long response;
    vector<uint8_t> data(nmtCommand);
    data.push_back(id);

    SendMessage(SetCanOpenMsg(0, 0 ,data) );

    return 0;

}

long CiA301CommPort::ReadNMT(const vector<uint8_t> & nmtCode)
{

    can_frame output;

    vector<uint8_t> nmtBoot(nmtCode);
    nmtBoot.push_back(id);
    long code=DataToInt(nmtBoot.data(),nmtBoot.size());
    cout<<" --> ReadNMT. Expecting: "  << std::hex << code << std::dec  << endl;

    for(long reps=0; reps<FIND_RETRY; reps++)
    {
        port->GetNMT(output.data, output.can_dlc);
        //cout << " Received: " << std::hex << input.id << std::dec << endl;
        //First check for the input.
        //If the response is the answer expected, continue
        if ( code == DataToInt(output.data,output.can_dlc) )
        {
            //cout << " comp: " << (input.id == expected_cobid) << endl;
            break;
        }
        else
        {
            cout << " ReadNMT still not received. Received: " << std::hex << DataToInt(output.data,output.can_dlc) << std::dec << endl;
        }

        if (reps==FIND_RETRY-1)
        {
            //fprintf(stderr, "Max retry reached: %u", reps);
            cerr << " ReadNMT Max retry reached: " << reps << endl;
            cout << " ReadNMT Max retry reached: " << reps << endl;

            return -1;
        }


    }
    //NMT
    cout<<"  <-- ReadNMT. Received: " << std::hex << code << std::dec << endl;
    cout << "ID: " << std::hex<< (int)output.data[output.can_dlc-1] << std::dec<< endl;


    return 0;
}

long CiA301CommPort::ReadErrorNMT()
{

    co_msg output;

    //NMT
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

        output.ff=FF_NORMAL; //normal frame
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
//    cout<<"input.dlc"<<input.dlc<<endl;
    output.dlc_co=input.dlc;
    output.id_co=input.id;
    output.rtr=input.rtr;
    output.ts=input.ts;

   //
    for( int i=0; i < 8; i++)
    {
        output.data_co[i] = input.data[i];
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
//    cout<<"SendMessage " << endl;
//    cout<<"sent cob id " << std::hex << input.id_co << std::dec <<" rtr: " << input.rtr << endl;
//            cout<<"sent data: ";
//            for (int i = 0; i < input.dlc_co; i++)
//            {

//                printf("%02x ",input.data_co[i]);
//            }
//            cout<<endl;
    if (CanOpenToCanBus(input,send_msg) < 0)
    {
        cout<<"Error al transformar el mensaje"<<endl;
    }

    else
    {
        //print can frame information
//        cout<<"sent can id " << (bitset<16>)send_msg.id << " rtr: " << send_msg.rtr << endl;

//        cout<<"sent data: ";
//        for (int i = 0; i < send_msg.dlc; i++)
//        {

//            printf("%02x ",send_msg.data[i]);
//        }
//        cout<<endl;


        /* Write the message to the Port */

        if(usesockets)
        {
            uint32_t tmpid = send_msg.id;
            uint16_t tmpdlc = send_msg.dlc;
            port->PutMsg(tmpid, send_msg.data, tmpdlc);

        }
        else
        {
            if(write(portFileDescriptor,&send_msg,sizeof(struct can_msg))!=sizeof(struct can_msg))
            {
                err(1, "Failed to send message");

            }
        }
    }
    return 0;
}

int CiA301CommPort::SendCanMessage(can_msg &input)
{
    //cout<<"SendCanMessage " << endl;

    //print can frame information
    //cout<<"sent can id " << (bitset<16>)send_msg.id << " rtr: " << send_msg.rtr << endl;

//    cout<<"sent data: ";
//    for (int i = 0; i < send_msg.dlc; i++)
//    {

//        printf("%02x ",send_msg.data[i]);
//    }
//    cout<<endl;

    /* Write the message to the Port */

    if(usesockets)
    {
        uint32_t tmpid = send_msg.id;
        port->PutMsg(tmpid, send_msg.data, send_msg.dlc);

    }
    else
    {
        if(write(portFileDescriptor,&send_msg,sizeof(struct can_msg))!=sizeof(struct can_msg))
        {
            err(1, "Failed to send message");

        }
    }


    return 0;
}

int CiA301CommPort::WaitForReadMessage(co_msg & output, unsigned int canIndex){

    can_msg input;
//    cout<<"WaitForReadMessage -----------" << endl;


#if USE_TIMEOUT
    if(read_timeout(portFileDescriptor,&input,USE_TIMEOUT)==0)
    {
        //err(1,"Could not read the message.");
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
//        //print can frame information
//        cout<<"received can id " << (bitset<16>)input.id << " rtr: " << input.rtr << endl;
//        cout<<"received data: ";
//        for (int i = 0; i < input.dlc; i++)
//        {

//            printf("%02x ",input.data[i]);
//        }
//        cout<<endl;
//                cout<<"received cob id " << std::hex << output.id_co << std::dec << " rtr: " << output.rtr << endl;
//                cout<<"received canopen data: ";
//                for (int i = 0; i < output.dlc_co; i++)
//                {

//                    printf("%02x ",output.data_co[i]);
//                }
//                cout<<endl;
//                cout<<"WaitForReadMessage End-----------" << endl;
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
int CiA301CommPort::ReadCobId(uint16_t expected_cobid, co_msg & output )
{



    vector<can_msg> otherMsgs(0);



    //cout << " Canid Received: " << std::hex << input.id << std::dec << endl;
    //    cout<<" --> ReadCobId. Expecting: "  << std::hex << expected_cobid << std::dec  << endl;

    for(long reps=0 ; reps<FIND_RETRY ;reps++)
    {
        GetMsg(input);

//        cout << "Expecting :"<< std::hex << expected_cobid << " Received: " << std::hex << input.id << std::dec << endl;
        //First check for the input.
        //If the response is the answer expected, continue
        if (input.id == expected_cobid)
        {
//            cout << " comp: " << (input.id == expected_cobid) << endl;
//            FlushBuffer();
            break;
        }
        //if not cobid but node id:
        if (GET_NODE_ID(input.id) == GET_NODE_ID(expected_cobid) )
        {
//            cout << " Cobid still not received. Received: " << std::hex << input.id << std::dec << endl;
            //if id, check if error
            //return -1;
        }
        else
        {
//            cout << " Cobid still not received. Received: " << std::hex << input.id << std::dec << endl;
            otherMsgs.push_back(input);
        }
        if (reps==FIND_RETRY-1)
        {
            cerr<<" ReadCobId Max retry reached " << endl;

            cout<<" ReadCobId Max retry reached " << endl;
            return -1;
        }


    }
    //GetMsg(input);

//    for (int i=0; i<otherMsgs.size(); i++)
//    {
//        cout << "resend number : " << i << " with bytes: " << otherMsgs[i].dlc;  ;
//        cout<<" message with cob id: " << std::hex << otherMsgs[i].id << std::dec <<" rtr: " << otherMsgs[i].rtr << endl;
//        for (int j = 0; j < otherMsgs[i].dlc; j++)
//        {

//            printf("%02x ",otherMsgs[i].data[j]);
//        }
        //TODO: find another way for this without sending again
        //SendCanMessage(otherMsgs[i]);
//    }

    //convert received data to canopen
    if(CanBusToCanOpen(input, output)!=0)
    {
       err(1,"error al convertir el mensaje");
       return -1;
    }
    else
    {

//        cout<<"  <-- ReadCobId. Received: " << std::hex << output.id_co << std::dec << endl;
//        cout << "ID: " << std::hex<< GET_NODE_ID(output.id_co) << std::dec<< endl;
        //cout << " rtr: " << output.rtr << endl;
//        cout<<"received canopen data: ";
//        for (int i = 0; i < output.dlc_co; i++)
//        {

//            printf("%02x ",output.data_co[i]);
//        }
//        cout<<endl;
        //TODO: use dlc here!!!
        return data4x8to32(&output.data_co[4],output.dlc_co);
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
        //err(1,"Port connection timed out");
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


long CiA301CommPort::GetMsg(can_msg &msg)
{

    if (usesockets)
    {
        uint32_t tmpid;
        uint8_t size;
        //doing it trough port object
        port->GetMsg(tmpid,msg.data,size);
        msg.dlc=size;
        msg.id= tmpid;
        return 0;
    }


#if USE_TIMEOUT

    timeval tv;
    //int readyMsg;

    //can_msg msg;

    //As USE_TIMEOUT are miliseconds, get seconds, then miliseconds
    tv.tv_sec=(int)USE_TIMEOUT/1000;
    tv.tv_usec=USE_TIMEOUT-(tv.tv_sec*1000);

    //use select to check readyness
    fd_set fds; // file descriptor set
    FD_ZERO(&fds);// clear the set
    FD_SET(portFileDescriptor,&fds);// put the fd in the set
    assert(FD_ISSET(portFileDescriptor,&fds));
    int readyMsg=select(portFileDescriptor+1,&fds,0,0,&tv);
    //cout<< "Number of ready ports: " << ret <<endl;

    if(readyMsg==0)/* timeout */
    {
        //err(1,"Port connection timed out");
        //cout<< "Bus empty. Messages in buffer: " << readyMsg <<endl;
        return 1;

    }
    else if (readyMsg<0)
    {
        err(readyMsg,"Numbered Error. See exit code");
        return readyMsg;

    }
    else
    {

        if(pread(portFileDescriptor,&msg,sizeof(struct can_msg), 0) != sizeof(struct can_msg))
        {
            //err(1,"Could not read the message.");
            cout << "Could not read the message." << endl;
            return -1;
        }


        //TODO *** Check if pread works with canbus ports at 0 offset

        //CanBusToCanOpen( msg , msg );
        return 0;
    }
#else //(NOT)USE_TIMEOUT
    if( pread(portFileDescriptor,&buf,sizeof(struct can_msg), 0) != sizeof(struct can_msg)  )
    {
        err(1,"read");
    }
    else
    {
        //CanBusToCanOpen( buf , msg );
        return 0;
    }
#endif //USE_TIMEOUT

}


uint32_t CiA301CommPort::data4x8to32(const uint8_t* in,int dlc)
{
    //TODO check for Sizes!!!!!
    //TODO: use dlc here!!!
    uint32_t retvalue;
    const vector<uint8_t> data_out={};

    for (int i=0;i<dlc;i++){
        //memcpy((uint8_t *)&(data_out+i),(uint8_t *)in+i,1);
        if(dlc>0){
            retvalue = in[i];
            retvalue = (retvalue << 8);
        }
        else{
            retvalue = data_out[i];
    }
//    uint32_t retvalue;
//    for(int i=0;i<dlc;i++){
//        if(dlc>0){
//    retvalue = data_out[i];
//    retvalue = (retvalue << 8);
//        }
//        else{
//         retvalue = data_out[i];
//      }
    }
    return retvalue;

    //return data_out;
}

ulong CiA301CommPort::DataToInt(const uint8_t* in, unsigned short size)
{
//    cout<<"SIZE "<<size<<endl;
    if ( (size<=0) | (size>4) )
    {
        return 0;
    }
    long retvalue = in[size-1];
//    cout<<"--- RETVALUE "<<retvalue<<endl;
    for (int i=size-2; i>=0; i--)
    {
    retvalue = (retvalue << 8) + in[i];
//    cout<<"----RETVALUE "<<retvalue<<endl;
    //cout <<"i" << i <<"v"<< retvalue <<endl;
    }
//    if (in[3]==0xFF && in[2]==0xFF)
//    {
//        //        cout<<"in[3]==0xFF"<<in[3]<<endl;
//        //        cout<<"in[2]==0xFF"<<in[2]<<endl;
//        return -(4294967296-retvalue);
//    }
    return retvalue;
}
