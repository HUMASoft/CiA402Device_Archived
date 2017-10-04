#ifndef CO_MSG_H
#define CO_MSG_H

/* Estrura del mensaje en CanOpen */
struct co_msg
{
    union{
    unsigned short id_co;
#ifdef HICO_LE
    struct {


        /* Node ID */
        unsigned short nodeID:7;

        /* Function Code */
        unsigned short fun_code:4;
    };
#elif defined(HICO_BE)
    struct {
        unsigned short fun_code:4;
        unsigned short nodeID:7;
        unsigned short rtr:1;
    };
#endif
    };
    /* Timestamp in microseconds */
    unsigned int ts;

    /* Remote transmission request flag */
    unsigned short rtr:1;
    unsigned short dlc_co;

    /* CAN message data */
    unsigned char data_co[8];

};

#endif // CO_MSG_H
