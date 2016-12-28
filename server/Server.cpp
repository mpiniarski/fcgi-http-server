#include "Server.h"
#include <iostream>

Server::Server() {
    int socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    listenSocket = new Socket(socketDescriptor);
    listenSocket->setReuseAddr();
    listenSocket->bindToLocalhost(8888);
    listenSocket->setListen(1);
}

void Server::listenForever() {
    while (true) {
        Socket clientSocket = listenSocket->acceptConnection();
        try {
            handleRequest(clientSocket);
        }
        catch (HttpException &exception) {
            clientSocket.sendMessage("Error: " + std::to_string(exception.getStatus()));
        }
//        FcgiCommunicator communicator = FcgiCommunicator();
    }
}

void Server::handleRequest(Socket &socketConnection) {
    try {
        std::string request = socketConnection.receiveMessage();
        std::cout << request << std::endl;
        socketConnection.sendMessage("Hello World!");
    }
    catch (RequestReceiveException &exception) {
        //TODO log
        throw InternalServerException();
    }
    catch (ResponseSendException exception) {
        //TODO log
    }
}

Server::~Server() {
    delete listenSocket;
}


