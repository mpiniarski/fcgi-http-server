#pragma once


#include "../socket/Socket.h"
#include "../content/fcgi/communication/FcgiCommunicator.h"
#include "../content/ContentProvider.h"
#include "http/HttpParser.h"
#include "../config/ConfigProvider.h"


class Server {
public:
    Server(HostAddress serverAddress, ContentProvider *dynamicContentProvider, int timeout);
    void listenForever();
    virtual ~Server();

private:
    Socket * listenSocket;
    ContentProvider *dynamicContentProvider;
    HttpParser* httpParser;
    int timeout;
    void sendResponse(Socket &socket, std::string response);
    void handleRequestWithTimeoutSupport(Socket &socketConnection);
    void handleRequest(Socket &socketConnection);
};

