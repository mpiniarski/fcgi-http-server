#include "Server.h"
#include "../fcgi.h"
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
    catch (SocketException& exception){
        throw FatalServerException(exception);
    }
}

void Server::listenForever() {
    FcgiCommunicator communicator = FcgiCommunicator();
    while (true) {
        Socket clientSocket = listenSocket->acceptConnection();
        try {
            handleRequest(clientSocket, communicator);
        }
        catch (HttpException &exception) {
            clientSocket.sendMessage("Error: " + std::to_string(exception.getStatus()));
        }
    }
}

void Server::handleRequest(Socket &socketConnection, FcgiCommunicator& communicator) {
    try {
        std::string request = socketConnection.receiveMessage();
        logger->debug("Received request:\n\n{}", request);
        communicator.sendRequest(request);
        FcgiResponse response = communicator.receiveResponse();
        if(response.protocolStatus == FCGI_REQUEST_COMPLETE){
            socketConnection.sendMessage(response.STDOUT);
            if(!response.STDERR.empty()){
                logger->warn("Fcgi errors:\n\n{}", response.STDERR);
            }
        }
        else{
            throw HttpException(503);
        }
    }
    catch (SocketRequestReceiveException &exception) {
        logger->error(exception.what());
        throw HttpException(500);
    }
    catch (SocketResponseSendException exception) {
        logger->error(exception.what());
    }
}

Server::~Server() {
    delete listenSocket;
}
