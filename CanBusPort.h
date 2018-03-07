#ifndef CANBUSPORT_H
#define CANBUSPORT_H


#include <string>
#include <vector>
#include <iostream>
#include <fcntl.h>
#include <err.h>


//#include "hico_api.h"
#include "candatatypes.h"
#include "PortBase.h"


using namespace std;
class CanBusPort : public PortBase
{
public:
    ///
    /// \brief CanBusPort: Empty constructor. Initialize default port "/dev/can0".
    ///
    CanBusPort();

    ///
    /// \brief CanBusPort: Initialization of one port given a device name.
    /// \param canPort: String with the name of system device.
    ///
    CanBusPort(string canPort);
    int getPortId();

    long SetFilter(uint32_t canId, uint32_t mask);
    long GetMsg(uint32_t &canId, uint8_t *data, uint8_t &size);
    long PutMsg(const uint32_t &canId, uint8_t * const data, const uint8_t size);
    long GetNMT(uint8_t * const data, uint8_t & size);
private:

    long Init(string canPort);


    //int portFD;

};

#endif // CANBUSPORT_H
