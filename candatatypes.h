#ifndef CANDATATYPES_H
#define CANDATATYPES_H



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

#endif // CANDATATYPES_H
