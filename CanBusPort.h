#ifndef CANBUSPORT_H
#define CANBUSPORT_H


#include <string>
#include <vector>
#include <fcntl.h>
#include <err.h>


#include "hico_api.h"


using namespace std;
class CanBusPort
{
public:
    ///
    /// \brief CanBusPort: Empty constructor. Initialize default port "/dev/can0".
    ///
    CanBusPort();

    ///
    /// \brief CanBusPort: Initialization of "n" ports starting at canPort0.
    /// \param number: Number of ports in the card.
    /// \param canPort: String with the name of system device.
    ///
    CanBusPort(string canPort);
    int getPortFileDescriptor() const;

private:

    long Init(string canPort);


    int canPortFileDesc;

};

#endif // CANBUSPORT_H
