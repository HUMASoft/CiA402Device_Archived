/*EM_LICENSE*/
/*
 * $Id: hico_api.h 1207 2009-05-11 13:36:51Z ny $
 * Author: Martin Nylund
 */

#ifndef _HICO_API_H
#define _HICO_API_H


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

#define BITRATE_10k	 0
#define BITRATE_20k	 1
#define BITRATE_50k	 2
#define BITRATE_100k	 3
#define BITRATE_125k	 4
#define BITRATE_250k	 5
#define BITRATE_500k	 6
#define BITRATE_800k	 7
#define BITRATE_1000k	 8

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



/**************************************************************************/
#define IOC_GET_CAN_STATUS	               _IOR (IOC_MAGIC, 40, uint32_t)
/**************************************************************************/
/* Returns the current status flags of the CAN node. */

/* Passive error - one or both of the tx and rx error counter has reached the
 * error warning limit (128) */
#define CS_ERROR_PASSIVE (1<<6)

/* Bus off error - Transmit error counter has reached its limiting value 255.
 * You can reset the error counter by setting the node in CM_RESET mode */
#define CS_ERROR_BUS_OFF (1<<7)

/* Following macros extract the current tx/rx error counter values from the
 * returned value */
#define CS_GET_RXERRCNT(status) ((status>>16)&0xff)
#define CS_GET_TXERRCNT(status) ((status>>24)&0xff)

/* Other flags int the status value are not of intrest for applications */


/**************************************************************************/
#define IOC_GET_BOARD_STATUS	               _IOR (IOC_MAGIC, 45, uint32_t)
/**************************************************************************/
/* Reads the current status of the board. In normal operation the value should
 * always be BS_RUNNING_OK. In case of an exception the values is something
 * else. See dpm.h for more information */
#define BS_RUNNING_OK 0xf2f20000


/**************************************************************************/
#define IOC_SET_FILTER	              _IOW (IOC_MAGIC, 50, struct can_filter)
/**************************************************************************/
/* Add an acceptance filter into the list of active acceptance filters. By
 * default, the acceptance masking is not active and all the messages are let
 * through */

/**************************************************************************/
#define IOC_CLEAR_FILTERS	                          _IO (IOC_MAGIC, 55)
/**************************************************************************/
/* Clear all acceptance filters for the node and set the acceptance in default
 * state (all messages let through)*/


/**************************************************************************/
#define IOC_MSGS_IN_RXBUF	                    _IOR (IOC_MAGIC, 60, int)
/**************************************************************************/
/* Return the number of messages that can be read from the boards receive
 * buffer */

/**************************************************************************/
#define IOC_MSGS_IN_TXBUF	                    _IOR (IOC_MAGIC, 61, int)
/**************************************************************************/
/* Return the number of messages that are written to the CAN nodes transmit
 * buffer */

/**************************************************************************/
#define IOC_GET_TXBUF_SIZE	                    _IOR (IOC_MAGIC, 62, int)
/**************************************************************************/
/* Size of transmit buffer */
/**************************************************************************/
#define IOC_GET_RXBUF_SIZE	                    _IOR (IOC_MAGIC, 63, int)
/**************************************************************************/
/* Size of the receive buffer */

/**************************************************************************/
#define IOC_RESET_TIMESTAMP	                          _IO (IOC_MAGIC, 65)
/**************************************************************************/
/* Reset the CAN message timestamp timer. Note that this command affects both
 * of the nodes.*/

/**************************************************************************/
#define IOC_GET_HW_ID	                      _IOR (IOC_MAGIC, 70, uint32_t)
/**************************************************************************/
/* read the boards hardware ID. The function returns one of the following
 * values */
/* HiCO.CAN-MiniPCI */
#define HW_HICOCAN_MPCI 0x10
/* HiCO.CAN-PCI-104 and PC/104+ */
#define HW_HICOCAN_PCI104 0x13
/* Unknown hardware Identifier */
#define HW_HICOCAN_UNKNOWN 0xff

/**************************************************************************/
#define IOC_GET_FW2_VERSION	             _IOR (IOC_MAGIC, 71, uint32_t)
/**************************************************************************/
/* Read version number for the board firmware (FW1 is the internal
 * bootloader and FW2 ist the actual firmware) */

/**************************************************************************/
#define IOC_GET_DRIVER_VERSION	             _IOR (IOC_MAGIC, 72, uint32_t)
/**************************************************************************/
/* Get version number of the driver */

/**************************************************************************/
#define IOC_GET_CAN_TYPE	             _IOR (IOC_MAGIC, 73, uint32_t)
/**************************************************************************/
/* Get CAN node type node_type gives information of the physical CAN
 * interface */
#define CAN_TYPE_EMPTY 0       /* Tranceiver not mounted on the PCB */
#define CAN_TYPE_HS 1          /* High-Speed tranceiver */
#define CAN_TYPE_FT 2          /* Fault Tolerant tranceiver */

/**************************************************************************/
#define IOC_GET_PCI104_POS	             _IOR (IOC_MAGIC, 75, uint32_t)
/**************************************************************************/
/* PCI-104 boards are identified with a jumpered value, which define their
 * location in the PCI-104 stack. This function returns a value from 0 to 3.
 * NOTE: this function doesn't have any meaning but for the PCI-104 boards */

/**************************************************************************/
#define IOC_GET_IOPIN_STATUS	               _IOR (IOC_MAGIC, 80, uint32_t)
/**************************************************************************/
/* Get the status of the can nodes IOPIN. On Fault Tolerant (FT) boards, this
 * is the BusError signal (low active -> '1' means no error). This values is
 * also saved to every received CAN telegram (see iopin in struct can_msg) */

#ifndef __QNX__
/**************************************************************************/
#define IOC_GET_ERR_STAT	       _IOR (IOC_MAGIC, 81, struct err_stat)
/**************************************************************************/
struct err_stat{
    uint32_t values[0x3f];
};
/**************************************************************************/
#define IOC_CLEAR_ERR_STAT	                         _IO (IOC_MAGIC, 82)
/**************************************************************************/
#endif


/**************************************************************************/
#define IOC_PRODUCTION_OK      _IO     (IOC_MAGIC, 101)
/**************************************************************************/
/* This command sets the board in "blink leds" mode. It is used in production
 * and in the example programs to signal that everythings ok. This ioctl call
 * has no use in normal CAN usage, and the board needs to be reset after
 * putting it into the "blink leds" mode */
/**************************************************************************/
#define IOC_GET_LPCBC_REV	             _IOR (IOC_MAGIC, 102, uint32_t)
/**************************************************************************/
/* This returns the onboard processors bootloader FW revsision number.*/


#if 0
/**************************************************************************/
#define IOC_SET_MODE                          _IOW (IOC_MAGIC, 65, uint32_t)
/**************************************************************************/
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

#define FF_NORMAL 0
#define FF_EXTENDED 1

struct can_filter{
    int type;
    union{
    uint32_t mask;
    uint32_t upper;
    };
    union{
    uint32_t code;
    uint32_t lower;
    };
};
#define FTYPE_AMASK 1
#define FTYPE_RANGE 2


#endif
