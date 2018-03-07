#include "CanBusPort.h"


//Constructor vacío (inicializa el can0)
CanBusPort::CanBusPort()
{
    Init( "/dev/can0" );
}

//Inicializa todos los puertos que introduzcas
CanBusPort::CanBusPort(string canPort)
{

    Init(canPort);

}

//Función para inicializar los puertos
long CanBusPort::Init(string canPort)
{
    int val;


    portId = open(canPort.c_str(), O_RDWR);

    if (portId<0){
        err(1, "could not open node '%s'",canPort.c_str());
    }

    /* Reset the board. Which node is used for this doesn't matter */
    if(ioctl(portId,IOC_RESET_BOARD)!=0){
        err(1, "could not reset board");
    }

    /* Set baudrate for both nodes */
    val=BITRATE_1000k;
    if(ioctl(portId,IOC_SET_BITRATE,&val)!=0){
        err(1, "could not set bitrate");
    }

    /* Start both CAN nodes */
    if(ioctl(portId,IOC_START)!=0){
        err(1, "IOC_START");
    }

    return 0;
}

int CanBusPort::getPortId()
{
    return portId;
}

long CanBusPort::SetFilter(uint32_t canId, uint32_t mask)
{
    cerr << "Not implemented" << endl;
    return -1;
}

long CanBusPort::GetMsg(uint32_t &canId, uint8_t *data, uint8_t &size)
{
    cerr << "Not implemented" << endl;
    return -1;

}

long CanBusPort::PutMsg(const uint32_t &canId, uint8_t * const data, const uint8_t size)
{
    cerr << "Not implemented" << endl;
    return -1;
}
long CanBusPort::GetNMT(uint8_t * const data, uint8_t &size)
{
    cerr << "Not implemented" << endl;
    return -1;
}
