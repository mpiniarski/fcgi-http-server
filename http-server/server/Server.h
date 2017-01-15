#pragma once


#include "../socket/Socket.h"
#include "../content/fcgi/communication/FcgiCommunicator.h"
#include "../content/ContentProvider.h"
#include "http/HttpParser.h"
#include "../config/ConfigProvider.h"


class Server {
public:
    Server(HostAddress serverAddress, ContentProvider *dynamicContentProvider, ContentProvider *staticContentProvider);

    void listenForever();
    virtual ~Server();

private:
    Socket * listenSocket;
    ContentProvider *dynamicContentProvider;
    ContentProvider *staticContentProvider;
    HttpParser* httpParser;
    void handleRequest(Socket &socketConnection);
    void sendResponse(Socket &socket, std::string response);
};

