#include "Server.h"
#include "../socket/exceptions.h"
#include "../content/exceptions.h"
#include "http/exceptions.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <thread>

static auto logger = spdlog::stdout_color_mt("Server");

Server::Server(HostAddress serverAddress, ContentProvider *dynamicContentProvider) {
    try {
        int socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
        listenSocket = new Socket(socketDescriptor);
        listenSocket->setReuseAddr();
        listenSocket->bindTo(serverAddress.ip, (uint16_t) serverAddress.port);
        listenSocket->setListen(1);

        this->dynamicContentProvider = dynamicContentProvider;
        this->httpParser = new HttpParser();
    }
    catch (SocketException &exception) {
        throw FatalServerException(exception);
    }
    logger->info("Created server at {}:{}", serverAddress.ip, std::to_string(serverAddress.port));
}

void Server::listenForever() {
    while (true) {
        Socket *clientSocket = listenSocket->acceptConnection();
        logger->debug("Accepted new connection");
        std::thread handleThread(&Server::handleRequest, this, std::ref(*clientSocket));
        handleThread.detach();
    }
}

void Server::handleRequest(Socket &socketConnection) {
    try {
        std::string request = socketConnection.receiveMessage();
        logger->debug("Received request:\n{}", request);
        HttpRequest httpRequest = httpParser->parseToHttpRequest(request); //TODO add parsing exception (400?)
        //TODO decide whether to use static or dynamic content provider
        std::string httpResponse = dynamicContentProvider->getResponse(
                httpRequest); // TODO add timeout exception (504?)
        //TODO validate response(?)
        socketConnection.sendMessage(httpResponse);
        logger->debug("Sent response:\n{}", httpResponse);
        delete (&socketConnection);
    }
    catch (ConnectionClosedException &exception) {
        logger->warn(exception.what());
    }
    catch (HttpParserException &exception) {
        logger->error(exception.what());
        HttpResponse response = HttpResponse(HTTP_VERSION_1_0, HTTP_400_BAD_REQUEST);
        sendResponse(socketConnection, httpParser->parseToStringResponse(response));
    }
    catch (SocketMessageSendException &exception) {
        logger->error(exception.what());
    }
    catch (SocketException &exception) {
        logger->error(exception.what());
        HttpResponse response = HttpResponse(HTTP_VERSION_1_0, HTTP_500_INTERNAL_SERVER_ERROR);
        sendResponse(socketConnection, httpParser->parseToStringResponse(response));
    }
    catch (ContentProviderRespondingException &exception) {
        logger->error(exception.what());
        HttpResponse response = HttpResponse(HTTP_VERSION_1_0, HTTP_500_INTERNAL_SERVER_ERROR);
        sendResponse(socketConnection, httpParser->parseToStringResponse(response));
    }
}

void Server::sendResponse(Socket &socket, std::string response) {
    try {
        socket.sendMessage(response);
    }
    catch (SocketMessageSendException &exception) {
        logger->warn(exception.what());
    }
}

Server::~Server() {
    delete listenSocket;
    delete dynamicContentProvider;
    delete httpParser;
}
