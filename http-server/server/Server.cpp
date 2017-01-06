#include "Server.h"
#include "../socket/exceptions.h"
#include "../content/exceptions.h"
#include "http/exceptions.h"
#include <iostream>
#include <spdlog/spdlog.h>

static auto logger = spdlog::stdout_color_mt("Server");

Server::Server(ContentProvider *dynamicContentProvider) {
    try {
        int socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
        listenSocket = new Socket(socketDescriptor);
        listenSocket->setReuseAddr();
        listenSocket->bindTo(8888);
        listenSocket->setListen(1);

        this->dynamicContentProvider = dynamicContentProvider;
        this->httpParser = new HttpParser();
    }
    catch (SocketException &exception) {
        throw FatalServerException(exception);
    }
}

void Server::listenForever() {
    Socket *clientSocket = NULL;
    while (true) {
            clientSocket = listenSocket->acceptConnection();
            handleRequest(*clientSocket);
            delete(clientSocket);
    }
}

void Server::handleRequest(Socket &socketConnection) {
    try {
        std::string request = socketConnection.receiveMessage();
        logger->debug("Received request:\n{}", request);
        HttpRequest httpRequest = httpParser->parseToHttpRequest(request); //TODO add parsing exception (400?)
        //TODO decide whether to use static or dynamic content provider
        std::string httpResponse = dynamicContentProvider->getResponse(httpRequest); // TODO add timeout exception (504?)
        //TODO validate response(?)
        socketConnection.sendMessage(httpResponse);
    }
    catch (HttpParserException& exception) {
        logger->error(exception.what());
        HttpResponse response = HttpResponse(HTTP_VERSION_1_0, HTTP_400_BAD_REQUEST);
        sendResponse(socketConnection, httpParser->parseToStringResponse(response));
    }
    catch (SocketMessageSendException& exception) {
        logger->error(exception.what());
    }
    catch (SocketException& exception){
        logger->error(exception.what());
        HttpResponse response = HttpResponse(HTTP_VERSION_1_0, HTTP_500_INTERNAL_SERVER_ERROR);
        sendResponse(socketConnection, httpParser->parseToStringResponse(response));
    }
    catch(ContentProviderRespondingException& exception){
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
