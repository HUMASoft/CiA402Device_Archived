#include "TestPort.h"


//Constructor vacío (inicializa el archivo Port0.txt)
TestPort::TestPort()
{
    Init( "Port0.txt" );
    //canPorts={0};
}

//Inicializa todos los puertos que introduzcas
TestPort::TestPort(string name)
{

    Init(name);

}


//Función para inicializar los puertos
long TestPort::Init(string name)
{
        portId = open(name.c_str(), O_RDWR | O_CREAT, 0744);

        if (portId<0)
        {
            err(1, "could not open node '%s'",name.c_str());
        }

    return 0;
}


long TestPort::ShowBuffer()
{

    return 0;
}

int TestPort::getPortId()
{
    return portId;
}
