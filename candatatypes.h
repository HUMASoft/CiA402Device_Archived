#ifndef CANDATATYPES_H
#define CANDATATYPES_H

#include<linux/can.h>

//masks used to get data
//get id from cob-id
#define GET_NODE_ID(cobid) ( cobid & 0x7f )
//#define CO_ID_MASK 0xfe


/* hicocan_mpci 'magic' number for the ioctl calls */
#ifdef __QNX__
 #define IOC_MAGIC _DCMD_MISC
 #include <ioctl.h>
 #include <devctl.h>
 #include <stdint.h>
#else /* Linux */
 #define IOC_MAGIC 'E'
 #ifndef __KERNEL__
  #include <sys/ioctl.h>
  #include <stdint.h>
 #endif
#endif

/* Macro for structure packing */
#define PACKED __attribute__((packed))


/* Uncomment one of these if the test below fails for some reason */
/* #define HICO_BE */

#if !defined(HICO_LE) && !defined(HICO_BE) && !defined(__KERNEL__)
 #ifdef __BYTE_ORDER
  #if __BYTE_ORDER == __LITTLE_ENDIAN
   #define HICO_LE
  #elif __BYTE_ORDER == __BIG_ENDIAN
   #define HICO_BE
  #endif
 #else
  #error -- could not detect endianess!
 #endif
#endif

#if defined(HICO_LE) && defined(HICO_BE)
 #error -- You cannot have both!
#endif



/**************************************************************************/
/* structure of a CAN messages                                            */
/**************************************************************************/
/* This structure is used for sending and receiving messages. Not all fields
 * have a meaning when sending (e.g. timestamp) */

#define MSG_DLC(msg)  (((msg)->fi&0xf)>>0)
#define MSG_RTR(msg)  (((msg)->fi&(1<<4))>>4)
#define MSG_FF(msg)   (((msg)->fi&(1<<5))>>5)
#define MSG_DOS(msg)  (((msg)->fi&(1<<6))>>6)
#define MSG_IOPIN(msg)(((msg)->fi&(1<<7))>>7)
#define MSG_NODE(msg) (((msg)->fi&(3<<8))>>8)

struct can_msg {
    union{
    uint16_t fi;
#ifdef HICO_LE
    struct {
        /* Data length 0 to 8 */
        uint16_t dlc:4;

        /* remote transmission request flag */
        uint16_t rtr:1;

        /* Frame format flag 0=normal, 1=extended */
        uint16_t ff:1;

        /* Data overrun status flag */
        uint16_t dos:1;

        /* io pin status or can error signal on fault tolerant can */
        uint16_t iopin:1;

        /* CAN node number which received the message */
        uint16_t node:2;

        uint16_t reserved:6;
        }PACKED;
#elif defined(HICO_BE)
    struct {
        uint16_t reserved:6;
        uint16_t node:2;
        uint16_t iopin:1;
        uint16_t dos:1;
        uint16_t ff:1;
        uint16_t rtr:1;
        uint16_t dlc:4;
        }PACKED;
#endif
    }PACKED;

    /* Timestamp in microseconds */
    uint32_t ts;

    /* CAN identifier */
    uint32_t id;

    /* CAN message data */
    uint8_t data[8];
}PACKED;


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







#define FF_NORMAL 0
#define FF_EXTENDED 1



/**************************************************************************/
#define IOC_RESET_BOARD                                   _IO (IOC_MAGIC, 1)
/**************************************************************************/
/* Reset the board. Note that this command affects both of the CAN nodes on
 * the board */

/**************************************************************************/
#define IOC_START                                         _IO (IOC_MAGIC, 5)
/**************************************************************************/
/* This will put the CAN node in active state - CAN telegrams are sent and
 * received */

/**************************************************************************/
#define IOC_START_PASSIVE                                 _IO (IOC_MAGIC, 10)
/**************************************************************************/
/* Start the CAN node in passive mode. Listen to the CAN bus traffic but don't
 * affect it in any way. CAN messages are received normally but transmission
 * is disabled */

/**************************************************************************/
#define IOC_START_BAUDSCAN                                _IO (IOC_MAGIC, 15)
/**************************************************************************/
/* This command will put the CAN node in baudscan mode. The node will probe
 * for the correct baudrate on the CAN bus, but doesn't affect the traffic in
 * any way. Of course, in order to the scan to succeed there has to be traffic
 * on the bus. The first succesfully received message is put in the boards
 * receive queue after which the node is in active state (like after givining
 * IOC_START command ) */

/**************************************************************************/
#define IOC_STOP                                         _IO (IOC_MAGIC, 20)
/**************************************************************************/
/* This command will put the CAN node in reset state. No CAN telegrams are
 * received or transmitted. */

/**************************************************************************/
#define IOC_GET_MODE                          _IOR(IOC_MAGIC, 25,  uint32_t)
/**************************************************************************/
/* Retrieve the nodes operation mode */

/* node is in baudscan mode */
#define CM_BAUDSCAN 1

/* node listens to the CAN bus traffic but don't affect it in any way */
#define CM_PASSIVE  2

/* Normal usage mode. CAN messages are received and transmitted */
#define CM_ACTIVE   3

/* In reset mode, no messages are received or transmitted. */
#define CM_RESET    4



/**************************************************************************/
#define IOC_SET_BITRATE	                       _IOW (IOC_MAGIC, 30, uint32_t)
/**************************************************************************/
/* Set the bitrate (aka. baudrate) for the given CAN nodes. Parameter is one
 * of the BITRATE_* macros below */

#define BITRATE_10k	  0
#define BITRATE_20k	  1
#define BITRATE_50k	  2
#define BITRATE_100k  3
#define BITRATE_125k  4
#define BITRATE_250k  5
#define BITRATE_500k  6
#define BITRATE_800k  7
#define BITRATE_1000k 8

/**************************************************************************/
#define IOC_SET_SJW_INCREMENT                  _IOW (IOC_MAGIC, 31, uint32_t)
/**************************************************************************/
/* Set a value which will be added to the Synchronisation Jump Width (SJW)
 * on every bitrate. value can be from 0 to 3, and default is 0. This
 * parameter might be useful to tweak if there are very long CAN cables or
 * some CAN nodes, which are not that accurate with the bitrate. The value
 * has to be set before calling IOC_SET_BITRATE. Normally you don't need
 * to set this value */

/**************************************************************************/
#define IOC_GET_BITRATE	                       _IOR (IOC_MAGIC, 35, uint32_t)
/**************************************************************************/
/* Get the bitrate index number. The value corresponds one of the above
 * defined bitrates BITRATE_*k */







#endif // CANDATATYPES_H
