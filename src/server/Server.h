#pragma once


#include "../socket/Socket.h"
#include "../content/fcgi/communication/FcgiCommunicator.h"
#include "../content/ContentProvider.h"
#include "http/HttpParser.h"


class Server {
public:
    Server(ContentProvider *dynamicContentProvider);

    void listenForever();
    virtual ~Server();

private:
    Socket * listenSocket;
    ContentProvider *dynamicContentProvider;
    HttpParser* httpParser;
    void handleRequest(Socket &socketConnection);
    void sendResponse(Socket &socket, std::string response);
};

