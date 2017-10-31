#include "CiA402DeviceICanbus.h"

//Constructor vacío (inicializa el can0)
CiA402DeviceICanbus::CiA402DeviceICanbus()
{
    Init( vector<int>(1) , "/dev/can" );
    //canPorts={0};
}

//Inicializa todos los puertos que introduzcas
CiA402DeviceICanbus::CiA402DeviceICanbus(long number, string canPort)
{

    canPorts.resize(number);
    for (int i=0; number; i++)
    {
        canPorts[i]=i;
    }
    Init(canPorts,canPort);

}

//Función para inicializar los puertos
long CiA402DeviceICanbus::Init(const vector<int> & new_canPorts, string canPort)
{
    int val;
    string portName;
    stringstream index;

    canPorts=new_canPorts;


    for(int canIndex=0;canIndex<canPorts.size();canIndex++)
    {
        index<<canIndex;
        portName=canPort+index.str();
        canPorts[canIndex] = open(portName.c_str(), O_RDWR);

        if (canPorts[canIndex]<0){
            err(1, "could not open node '%s'",portName.c_str());
        }

        /* Reset the board. Which node is used for this doesn't matter */
        if(ioctl(canPorts[canIndex],IOC_RESET_BOARD)!=0){
        err(1, "could not reset board");
        }

        /* Set baudrate for both nodes */
        val=BITRATE_1000k;
        if(ioctl(canPorts[canIndex],IOC_SET_BITRATE,&val)!=0){
        err(1, "could not set bitrate");
        }

        /* Start both CAN nodes */
        if(ioctl(canPorts[canIndex],IOC_START)!=0){
        err(1, "IOC_START");
        }
    }

    return 0;
}

//Funcion para construir mensaje en CanBus
can_msg CiA402DeviceICanbus::SetCanMsg(can_msg & msg, uint8_t msg_start[])
{
    msg.ff = FF_NORMAL;
    msg.id = 0x00;//x0B; 00 mensaje de red
    //tx_msg.id = 0x08;
    msg.dlc = 2;
    memcpy(msg.data, msg_start, 2*sizeof(uint8_t));
    for(int i=0;i<msg.dlc;i++){
    msg.data[i]=msg_start[i];
    }
    return msg;
}

/**
 * @brief CiA402DeviceICanbus::SetCanOpenMsg : Constructs canopen message from parameters
 * @param id_co: cob id canopen parameter.
 * @param rtr: request for remote.
 * @param msg_start : canopen data frame.
 * @return : canopen constructed message in co_msg data type.
 */

co_msg CiA402DeviceICanbus::SetCanOpenMsg(unsigned short id_co, unsigned short rtr, vector<uint8_t> coDataFrame){

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


/* Transforma mensaje de canopen a can y lo envía a can0 */
int CiA402DeviceICanbus::SendMessage(co_msg input, unsigned int canIndex)
{

    if (co2c(input,send_msg) < 0){
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
        if(write(canPorts[canIndex],&send_msg,sizeof(struct can_msg))!=sizeof(send_msg)){
        err(1, "Failed to send message");

        }

    }
    return 0;
}

int CiA402DeviceICanbus::WaitForReadMessage(co_msg & output, unsigned int canIndex){
#define USE_TIMEOUT 200
    can_msg input;
    //output.id_co=0;
    #if USE_TIMEOUT
        if(read_timeout(canPorts[canIndex],&input,5000)==0){
        err(1,"timeout - could not read the message. Check the cable!");
        return -1;
        }

    #else
        if(read(canPorts[canIndex],&input,sizeof(struct can_msg) !=sizeof(input)){
        err(1,"read");
        }
    #endif



    if(c2co(input, output)!=0){
       err(1,"error al convertir el mensaje");
       return -1;
    }
    else{
        //print can frame information
        cout<<"received can id " << (bitset<16>)input.id << " rtr: " << input.rtr << " ( " << std::hex << input.id << " )"<< endl;
        cout<<"received data: ";
        for (int i = 0; i < input.dlc; i++) {

            printf("%02x ",input.data[i]);
        }
        cout<<endl;
                cout<<"received canopen id " << (bitset<16>)output.id_co << " rtr: " << output.rtr << endl;
                cout<<"received canopen data: ";
                for (int i = 0; i < output.dlc_co; i++) {

                    printf("%02x ",output.data_co[i]);
                }
                cout<<endl;
    }
    return 0;
}

long CiA402DeviceICanbus::co2c(const co_msg & input, can_msg & output)
        {
                //creo que no hay que desplazar
                output.id=input.id_co;//>>1;


                //output.fi=input.id_co;
                //output.fi<<=4;
                //output.fi+=input.dlc_co;
                output.dlc=input.dlc_co;
                output.rtr=input.rtr;

                output.ff=FF_NORMAL;
                output.ts=input.ts;
                // creo que en vez de 8 poner dlc
                for( int i=0; i < 8; i++)
        {
                output.data[i] = input.data_co[i];
}



                return 0;
}
/* Función para convertir de can a canopen */
long CiA402DeviceICanbus::c2co(const can_msg & input, co_msg & output)
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

int CiA402DeviceICanbus::read_timeout(int fd, struct can_msg *buf, unsigned int timeout)
{
    fd_set fds;
    struct timeval tv;
    int sec,ret;
    FD_ZERO(&fds);

    sec=timeout/1000;
    tv.tv_sec=sec;
    tv.tv_usec=(timeout-(sec*1000))*1000;

    FD_SET(fd,&fds);

    ret=select(fd+1,&fds,0,0,&tv);
    if(ret==0){
    return 0; /* timeout */
    } else if (ret<0) {
    return errno;
    } else {
    assert(FD_ISSET(fd,&fds));
    ret=read(fd,buf,sizeof(struct can_msg));

    return ret;
    }
}
