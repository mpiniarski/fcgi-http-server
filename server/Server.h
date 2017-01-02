#pragma once


#include "Socket.h"
#include "FcgiCommunicator.h"



class Server {
public:
    Server();

    void listenForever();
    virtual ~Server();

private:
    Socket * listenSocket = NULL;
    void handleRequest(Socket &socketConnection, FcgiCommunicator &communicator);
};

