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

SocketCanPort::~SocketCanPort()
{
    //shutdown(portFD,0);
    close(portFD);

}

long SocketCanPort::SetFilter(uint32_t canId, uint32_t mask)
{
    //check for socket
    if(portFD<=0)
    {
        cerr << "Socket not open" << endl;
        return -1;
    }
    //filter add
    can_filter rfilter1[1];
    rfilter1[0].can_id   = canId;
    rfilter1[0].can_mask = mask;
//    rfilter1[1].can_id   = 0x000;
//    rfilter1[1].can_mask = 0x7FF;
    setsockopt(portFD, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter1, sizeof(rfilter1));
    return 0;
}

long SocketCanPort::GetNMT(uint8_t *data, uint8_t & size)
{
    //get a message from canbus and store in canId and data out parameters.
    //in socketcan, only one device will be listened, given a port, trough SetFilter
    //blocking. It will wait until filtered message arrives
    //cout << " (buff_size) " << (buff_size) << endl;
    //Poll data only if not buffer available.

    if (buffSizeNMT<=0)
    {
        buffSizeNMT = poll(poll_setNMT, 1, timeoutPoll);
        cout << " (buffSizeNMT) " << (buffSizeNMT) << endl;
        cout << " (revents 0) " <<hex << (poll_setNMT[0].revents) << dec << endl;

        if(buffSizeNMT<0)
        {
            cout << ("Error in poll read") << endl;
            return -2;

        }
        if(buffSizeNMT==0)
        {
            cout << ("Timeout in poll read") << endl;

            //perror("Timeout in poll read");
            return -1;

        }
    }


    nbytes = read(portNMT, &frame, sizeof(struct can_frame));


    //return second parameter
    memcpy ( data, frame.data, frame.can_dlc );
    size =frame.can_dlc;
    buffSizeNMT--;

    return (buffSizeNMT);
}

long SocketCanPort::GetMsg(uint32_t &canId, uint8_t *data, uint8_t size)
{
    //get a message from canbus and store in canId and data out parameters.
    //in socketcan, only one device will be listened, given a port, trough SetFilter
    //blocking. It will wait until filtered message arrives
    //cout << " (buff_size) " << (buff_size) << endl;
    //Poll data only if not buffer available.
    if (buffSizeFD<=0)
    {
        buffSizeFD = poll(poll_setFD, 1, timeoutPoll);
        cout << " (buffSizeFD) " << (buffSizeFD) << endl;
        cout << " (revents 0) " <<hex << (poll_setFD[0].revents) << dec << endl;

        if(buffSizeFD<0)
        {
            cout << ("Error in poll read") << endl;
            return -2;

        }
        if(buffSizeFD==0)
        {
            cout << ("Timeout in poll read") << endl;

            //perror("Timeout in poll read");
            return -1;

        }
    }


    nbytes = read(portFD, &frame, sizeof(struct can_frame));

    //return first parameter
    canId = frame.can_id;
    //return second parameter
    memcpy ( data, frame.data, frame.can_dlc );
    buffSizeFD--;

    return (buffSizeFD);




}

long SocketCanPort::PutMsg(const uint32_t &canId, uint8_t * const data, uint8_t const data_size)
{
    frame.can_id = canId;

    memcpy ( frame.data,  data, data_size );
    frame.can_dlc = data_size;
//    cout << "PutMsg: " << hex << frame.can_id << dec << "  | ";
//    cout << "data: " << hex << frame.data[0] << dec << endl;

    nbytes = write(portFD, &frame, sizeof(struct can_frame) );

    /* send frame */
    if (nbytes != sizeof(struct can_frame))
    {
        perror("Write Failed");
        return 1;
    }
    return nbytes;

}


long SocketCanPort::Init(string canPort)
{
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    portFD  = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    portNMT  = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if(portFD < 0)
    {
        perror("Error while opening socket \n "
               "try: sudo ip link set can0 up txqueuelen 1000 type can bitrate 1000000");
        return -1;
    }

    canPort.copy(ifr.ifr_name,canPort.size());
    ifr.ifr_name[canPort.size()]='\0';
    //printf("%s ifr %d\n", ifr.ifr_name, ifr.ifr_ifindex);

    ioctl(portFD, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    printf("%s at index %d\n", canPort.c_str(), ifr.ifr_ifindex);

    if(bind(portFD, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error in socket bind");
        return -2;
    }

    if(bind(portNMT, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error in socket bind");
        return -2;
    }

    //check for socket
    if(portNMT<=0)
    {
        cerr << "Socket not open" << endl;
        return -1;
    }
    //filter add
    can_filter rfilter2[1];
    rfilter2[0].can_id   = 0x000;
    rfilter2[0].can_mask = 0x7FF;
    setsockopt(portNMT, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter2, sizeof(rfilter2));

    poll_setFD[0].fd = portFD;
    poll_setFD[0].events = POLLIN | POLLERR | POLLPRI;
    poll_setNMT[0].fd = portNMT;
    poll_setNMT[0].events = POLLIN | POLLERR | POLLPRI;


    timeoutPoll = 10000;

    buffSizeFD =0;
    buffSizeNMT =0;

    return 0;
}
