#include "Server.h"
#include "FcgiCommunicator.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


Server::Server() {
    int result;

    this->listenSocket = socket(PF_INET, SOCK_STREAM, 0);
    setReuseAddr(this->listenSocket);
    struct sockaddr_in myAddr;
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(8888);
    myAddr.sin_addr.s_addr = INADDR_ANY;

    result = bind(this->listenSocket, (sockaddr *) &myAddr, sizeof myAddr);
    if (result == -1) {
        throw FatalServerException(errno);
    }

    int connectionSockets = 1;
    result = listen(this->listenSocket, connectionSockets);
    if (result == -1) {
        throw FatalServerException(errno);
    }

}

void Server::listenForever() {
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(sockaddr_in);
        int socketDescriptor = accept(this->listenSocket, (sockaddr *) &clientAddr, &clientAddrSize);
        if (socketDescriptor == -1) {
            throw FatalServerException(errno);
        }
        CommunicationSocket clientSocket = CommunicationSocket(socketDescriptor);
        try {
            handleRequest(clientSocket);
        }
        catch (HttpException &exception) {
            clientSocket.sendResponse("Error: " + std::to_string(exception.getStatus()));
        }
        FcgiCommunicator communicator = FcgiCommunicator();
    }
}

void Server::handleRequest(CommunicationSocket &socketConnection) {
    try {
        std::string request = socketConnection.receiveRequest();
        std::cout << request << std::endl;
        socketConnection.sendResponse("Hello World!");
    }
    catch (RequestReceiveException &exception) {
        //TODO log
        throw InternalServerException();
    }
    catch (ResponseSendException exception) {
        //TODO log
    }
}


void Server::setReuseAddr(int socketDescriptor) {
    const int flagValue = 1;
    int result = setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &flagValue, sizeof(flagValue));
    if (result == -1) {
        throw FatalServerException(errno);
    }
}

Server::~Server() {
    close(listenSocket);
}
