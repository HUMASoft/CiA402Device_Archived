#ifndef TESTPORT_H
#define TESTPORT_H

#include <string>
#include <vector>
#include <fcntl.h>
#include <err.h>

#include "PortBase.h"

using namespace std;

class TestPort : public PortBase
{
public:
    TestPort();
    TestPort(string Port);

    long ShowBuffer();
    int getPortId();

private:
    long Init(string name);



};

#endif // TESTPORT_H
