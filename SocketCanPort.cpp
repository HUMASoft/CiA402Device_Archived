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

long SocketCanPort::GetMsg(uint32_t &canId, uint8_t *data, uint8_t size)
{
    //get a message from canbus and store in canId and data out parameters.
    //in socketcan, only one device will be listened, given a port, trough SetFilter
    //blocking. It will wait until filtered message arrives
    //cout << " (buff_size) " << (buff_size) << endl;
    //Poll data only if not buffer available.
    if (buff_size<=0)
    {
        buff_size = poll(poll_set, 2, timeoutPoll);
        cout << " (new buff_size) " << (buff_size) << endl;
        cout << " (revents 0) " <<hex << (poll_set[0].revents) << dec << endl;
        cout << " (new buff_size) " << (buff_size) << endl;
        cout << " (revents 1) " <<hex << (poll_set[1].revents) << dec << endl;

        if(buff_size<0)
        {
            cout << ("Error in poll read") << endl;
            return -2;

        }
        if(buff_size==0)
        {
            cout << ("Timeout in poll read") << endl;

            //perror("Timeout in poll read");
            return -1;

        }
    }

    //TODO: redo it. quick and dirty!!!
    //better do in separate functions.
    if (poll_set[0].revents==0)
    {
        nbytes = read(portNMT, &frame, sizeof(struct can_frame));
    }
    else
    {
        nbytes = read(portFD, &frame, sizeof(struct can_frame));
    }
    //return first parameter
    canId = frame.can_id;
    //return second parameter
    memcpy ( data, frame.data, frame.can_dlc );
    buff_size--;

    return (buff_size);




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
    ifr.ifr_name[canPort.size()]=NULL;
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

    poll_set[0].fd = portFD;
    poll_set[0].events = POLLIN | POLLERR | POLLPRI;
    poll_set[1].fd = portNMT;
    poll_set[1].events = POLLIN | POLLERR | POLLPRI;


    timeoutPoll = 3000;

    buff_size =0;

    return 0;
}
