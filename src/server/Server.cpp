#include "Server.h"
#include "../socket/exception/exceptions.h"
#include "http/HttpResponder.h"
#include <iostream>
#include <spdlog/spdlog.h>

static auto logger = spdlog::stdout_color_mt("Server");

Server::Server(ContentProvider *dynamicContentProvider) {
    try {
        this->dynamicContentProvider = dynamicContentProvider;
        int socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
        listenSocket = new Socket(socketDescriptor);
        listenSocket->setReuseAddr();
        listenSocket->bindTo(8888);
        listenSocket->setListen(1);
    }
    catch (SocketException &exception) {
        throw FatalServerException(exception);
    }
}

void Server::listenForever() {
    HttpResponder httpResponder = HttpResponder();
    Socket *clientSocket = NULL;
    while (true) {
        try {
            clientSocket = listenSocket->acceptConnection();
            handleRequest(*clientSocket);
            delete(clientSocket);
        }
        catch (SocketException &exception) {
            delete(clientSocket);
            throw FatalServerException(exception);
        }
        catch (HttpException &exception) {
            HttpResponse response = HttpResponse();
            response.status = exception.getStatus();
            httpResponder.sendResponse(*clientSocket, response);
            delete(clientSocket);
        }
    }
}

void Server::handleRequest(Socket &socketConnection) {
    try {
        std::string request = socketConnection.receiveMessage();
        logger->debug("Received request:\n\n{}", request);
        //TODO parse request to HttpRequest + parsing exception (400?)
        HttpRequest httpRequest = HttpRequest();
        //TODO decide whether to use static or dynamic content provider
        std::string httpResponse = dynamicContentProvider->getResponse(httpRequest); // TODO + Exception (500?, 504?)
        //TODO* validate response
        //send response to client:
        socketConnection.sendMessage(httpResponse);
    }
    catch (SocketResponseSendException exception) {
        logger->error(exception.what());
    }
    catch (SocketException& exception){
        logger->error(exception.what());
        throw HttpException(HTTP_500_INTERNAL_SERVER_ERROR);
    }
}

Server::~Server() {
    delete listenSocket;
    delete dynamicContentProvider;
}
