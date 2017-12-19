#include "SocketCanPort.h"

//Constructor vacío (inicializa el can0)

SocketCanPort::SocketCanPort()
{
    Init( "can0" );
}

//Inicializa todos los puertos que introduzcas

SocketCanPort::SocketCanPort(string canPort)
{

    Init(canPort);

}


long SocketCanPort::Init(string canPort)
{
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    if((portFD  = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Error while opening socket \n "
               "try: sudo ip link set can0 up txqueuelen 1000 type can bitrate 1000000");        return -1;
    }

    canPort.copy(ifr.ifr_name,canPort.size());

            ioctl(portFD, SIOCGIFINDEX, &ifr);

            addr.can_family  = AF_CAN;
            addr.can_ifindex = ifr.ifr_ifindex;

            printf("%s at index %d\n", canPort.c_str(), ifr.ifr_ifindex);

            if(bind(portFD, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("Error in socket bind");
                return -2;
            }

    return 0;
}
