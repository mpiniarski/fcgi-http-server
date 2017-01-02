#include "Server.h"
#include "fcgi.h"
#include "exception/exceptions.h"
#include <iostream>
#include <spdlog/spdlog.h>

static auto logger = spdlog::stdout_color_mt("Server");

Server::Server() {
    try {
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
    FcgiCommunicator communicator = FcgiCommunicator();
    HttpResponder httpResponder = HttpResponder();
    Socket *clientSocket = NULL;
    while (true) {
        try {
            clientSocket = listenSocket->acceptConnection();
            handleRequest(*clientSocket, communicator);
            delete(clientSocket);
        }
        catch (SocketException &exception) {
            delete(clientSocket);
            throw FatalServerException(exception);
        }
        catch (HttpException &exception) {
            HttpResponse response = HttpResponse(exception.getStatus());
            httpResponder.sendResponse(*clientSocket, response);
            delete(clientSocket);
        }
    }
}

void Server::handleRequest(Socket &socketConnection, FcgiCommunicator &communicator) {
    try {
        std::string request = socketConnection.receiveMessage();
        logger->debug("Received request:\n\n{}", request);
        communicator.sendRequest(request);
        FcgiResponse response = communicator.receiveResponse();
        if (response.protocolStatus == FCGI_REQUEST_COMPLETE) {
            socketConnection.sendMessage(response.STDOUT);
            if (!response.STDERR.empty()) {
                logger->warn("Fcgi errors:\n\n{}", response.STDERR);
            }
        } else if (response.protocolStatus == FCGI_CANT_MPX_CONN || response.protocolStatus == FCGI_OVERLOADED){
            throw HttpException(HTTP_503_SERVICE_UNAVAILABLE);
        }
        else if (response.protocolStatus == FCGI_UNKNOWN_ROLE){
            throw HttpException(HTTP_500_INTERNAL_SERVER_ERROR);
        }
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
}
