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
    close(portId);

}

long SocketCanPort::SetFilter(uint32_t canId, uint32_t mask)
{
    //check for socket
    if(portId<=0)
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
    setsockopt(portId, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter1, sizeof(rfilter1));
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
        cout << " (revents 0) " << hex << (poll_setNMT[0].revents) << dec << endl;

        if(buffSizeNMT<0)
        {
            cout << ("Error in poll read") << endl;
            size = 0;
            data[0] = '\0';
            return -2;

        }
        if(buffSizeNMT==0)
        {
            cout << ("Timeout in poll read in the port ") <<  getPortId() << endl;
            size = 0;
            data[0] = '\0';
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



long SocketCanPort::GetMsg(uint32_t &canId, uint8_t *data, uint8_t &size)
{
    //get a message from canbus and store in canId and data out parameters.
    //in socketcan, only one device will be listened, given a port, trough SetFilter
    //blocking. It will wait until filtered message arrives
    //cout << " (buff_size) " << (buff_size) << endl;

    //Poll data only if not buffer available.
    if (buffSizeId<=0)
    {
        buffSizeId = poll(poll_setId, 1, timeoutPoll);
//        cout << " (buffSizeId) " << (buffSizeId) << endl;
//        cout << " (revents 0) " << hex << (poll_setId[0].revents) << dec << endl;

        if(buffSizeId<0)
        {
            cout << ("Error in poll read") << endl;
            size=0;
            data[0] = '\0';
            return -2;

        }
        if(buffSizeId==0)
        {
            cout << ("Timeout in poll read in the port ")<<  getPortId() << endl;
            size=0;
            data[0] = '\0';
            //perror("Timeout in poll read");
            return -1;

        }
    }


    nbytes = read(portId, &frame, sizeof(struct can_frame));

    //return first parameter
    canId = frame.can_id;
    //return second parameter
    memcpy ( data, frame.data, frame.can_dlc );
    //return third parameter
    size=frame.can_dlc;
    buffSizeId--;

    return (buffSizeId);




}

long SocketCanPort::PutMsg(const uint32_t &canId, uint8_t * const data, uint8_t const data_size)
{
    //set frame
    frame.can_id = canId;
    memcpy ( frame.data,  data, data_size );
    frame.can_dlc = data_size;


    nbytes = write(portId, &frame, sizeof(struct can_frame) );
    /* send frame */
    if (nbytes != sizeof(struct can_frame))
    {

        fprintf(stderr, "Write Failed in Port %d ", portId);
        //perror("Write Failed", portId);
        cerr << "PutMsg: " << hex << frame.can_id << dec << "  | ";
        cerr << "data[0]: " << hex << frame.data[0] << dec << "  | ";
        cerr << "nbytes: " << nbytes << endl;
        return 1;
    }
    return nbytes;

}


long SocketCanPort::Init(string canPort)
{
    struct sockaddr_can addr; //used in bind
    struct can_frame frame;
    struct ifreq ifr; //used in ioctl and addr

    portType = 2;
    //Open sockets.
    //socket variables
    //printf("%s at index %d\n", canPort.c_str(), ifr.ifr_ifindex);
    canPort.copy(ifr.ifr_name,canPort.size());
    ifr.ifr_name[canPort.size()]='\0';
    addr.can_family  = AF_CAN;


    //PortId: This socket will only get messages for some id. Used by GetMsg Function.
    //Filter sets in SetFilter function by caller (id unknown for this class).
    portId  = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(portId < 0)
    {
        perror("Error while opening socket. Is there a connection available? \n "
               "Try: sudo ip link set can0 up txqueuelen 1000 type can bitrate 1000000");
        return -1;
    }
    //get and show index
    ioctl(portId, SIOCGIFINDEX, &ifr);
//    printf("%s portId at index %d\n", ifr.ifr_name, ifr.ifr_ifindex);
    addr.can_ifindex = ifr.ifr_ifindex;
    if(bind(portId, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error in socket bind");
        return -2;
    }
//    printf("portId index %d\n", portId);



    //PortNMT: This socket will only get messages with cobid=0x000 (NMT). Used by GetNMT Function.
    portNMT  = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(portNMT < 0)
    {
        perror("Error while opening socket. Is there a connection available? \n "
               "Try: sudo ip link set can0 up txqueuelen 1000 type can bitrate 1000000");
        return -1;
    }
    //get and show index
    ioctl(portNMT, SIOCGIFINDEX, &ifr);
//    printf("%s portNMT at index %d\n", ifr.ifr_name, ifr.ifr_ifindex);
    addr.can_ifindex = ifr.ifr_ifindex;
    if(bind(portNMT, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Error in socket bind");
        return -2;
    }
//    printf("portNMT index %d\n", portNMT);
    //Add nmt filters (get only 0x000 cobid's)
    can_filter rfilterNMT[1];
    rfilterNMT[0].can_id   = 0x000;
    rfilterNMT[0].can_mask = 0x7FF;
    setsockopt(portNMT, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilterNMT, sizeof(rfilterNMT));



    //Set poll objects for portId
    poll_setId[0].fd = portId;
    poll_setId[0].events = POLLIN | POLLERR | POLLPRI;

    //Set poll objects for portNMT
    poll_setNMT[0].fd = portNMT;
    poll_setNMT[0].events = POLLIN | POLLERR | POLLPRI;


    //TODO:get as parameter??
    timeoutPoll = 10000;

    //Init buffers
    buffSizeId =0;
    buffSizeNMT =0;

    return 0;
}
