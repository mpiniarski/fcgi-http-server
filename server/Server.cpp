#include "Server.h"
#include "FcgiCommunicator.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <spdlog/spdlog.h>

static auto logger = spdlog::stdout_color_mt("Server");

Server::Server() {
    int socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    listenSocket = new Socket(socketDescriptor);
    listenSocket->setReuseAddr();
    listenSocket->bindToLocalhost(8888);
    listenSocket->setListen(1);
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

void Server::handleRequest(Socket &socketConnection, FcgiCommunicator communicator) {
    try {
        std::string request = socketConnection.receiveMessage();
        logger->debug("Received request:\n{}", request);
        communicator.sendRequest(request);
        socketConnection.sendMessage("Hello World!");
    }
    catch (RequestReceiveException &exception) {
        logger->error(exception.what());
        throw InternalServerException();
    }
    catch (ResponseSendException exception) {
        logger->error(exception.what());
    }
}

Server::~Server() {
    delete listenSocket;
}
