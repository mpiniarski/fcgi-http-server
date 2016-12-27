#include <string>
#pragma once

class FcgiComunicator {
public:
    FcgiComunicator();
    virtual ~FcgiComunicator();

private:
    int tcpSocket;
};

