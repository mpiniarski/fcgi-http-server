#pragma once


#include "../socket/Socket.h"
#include "../content/fcgi/FcgiCommunicator.h"
#include "../content/ContentProvider.h"


class Server {
public:
    Server(ContentProvider *dynamicContentProvider);

    void listenForever();
    virtual ~Server();

private:
    Socket * listenSocket = NULL;
    ContentProvider *dynamicContentProvider;
    void handleRequest(Socket &socketConnection);
};

