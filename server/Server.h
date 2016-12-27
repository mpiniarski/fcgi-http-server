#pragma once


#include "CommunicationSocket.h"

class Server {
public:
    Server();

    virtual ~Server();

    void listenForever();

private:
    int listenSocket;

    void handleRequest(CommunicationSocket &socketConnection);
    void setReuseAddr(int socketDescriptor);
};

