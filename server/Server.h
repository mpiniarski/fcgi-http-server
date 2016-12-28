#pragma once


#include "Socket.h"

class Server {
public:
    Server();

    void listenForever();


    virtual ~Server();

private:
    Socket * listenSocket = NULL;

    void handleRequest(Socket &socketConnection);
};

