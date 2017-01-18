#pragma once


#include "../socket/Socket.h"
#include "../content/fcgi/communication/FcgiCommunicator.h"
#include "../content/ContentProvider.h"
#include "http/HttpParser.h"
#include "../config/ConfigProvider.h"
#include <regex>


class Server {
public:
    Server(HostAddress serverAddress, ContentProvider *dynamicContentProvider,
           ContentProvider *staticContentProvider, std::vector<std::string> dynamicUriPatterns, int timeout);

    void listenForever();
    virtual ~Server();

private:
    Socket * listenSocket;
    ContentProvider *dynamicContentProvider;
    ContentProvider *staticContentProvider;
    HttpParser* httpParser;
    int timeout;
    void sendResponse(Socket &socket, std::string response);
    void handleRequestWithTimeoutSupport(Socket &socketConnection);
    void handleRequest(Socket &socketConnection);
    std::vector<std::regex> dynamicUriPatterns;

    bool isMatchingToDynamicUri(const HttpRequest &httpRequest) const;
};

