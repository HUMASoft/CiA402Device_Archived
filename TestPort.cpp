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
        portFileDescriptor = open(name.c_str(), O_RDWR | O_CREAT);

        if (portFileDescriptor<0)
        {
            err(1, "could not open node '%s'",name.c_str());
        }

    return 0;
}
