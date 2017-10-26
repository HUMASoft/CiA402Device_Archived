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


    canPortFileDesc = open(canPort.c_str(), O_RDWR);

    if (canPortFileDesc<0){
        err(1, "could not open node '%s'",canPort.c_str());
    }

    /* Reset the board. Which node is used for this doesn't matter */
    if(ioctl(canPortFileDesc,IOC_RESET_BOARD)!=0){
        err(1, "could not reset board");
    }

    /* Set baudrate for both nodes */
    val=BITRATE_1000k;
    if(ioctl(canPortFileDesc,IOC_SET_BITRATE,&val)!=0){
        err(1, "could not set bitrate");
    }

    /* Start both CAN nodes */
    if(ioctl(canPortFileDesc,IOC_START)!=0){
        err(1, "IOC_START");
    }

    return 0;
}
