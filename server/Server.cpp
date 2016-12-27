#include <stdlib.h>
#include "Server.h"

Server::Server() {
    int error;

    this->tcpSocket = socket(PF_INET, SOCK_STREAM, 0);
    setReuseAddr(this->tcpSocket);

    struct sockaddr_in myAddr;
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons (8888);
    myAddr.sin_addr.s_addr = INADDR_ANY;

    error = bind(this->tcpSocket, (sockaddr*) &myAddr, sizeof myAddr);
    if(error == -1){
        perror("Bind");
        exit(1);
    }

    int connectionSockets = 1;
    error = listen(this->tcpSocket, connectionSockets);
    if(error == -1){
        perror("Listen");
        exit(1);
    }
}

void Server::listenForever() {
    int error;

    while(true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(sockaddr_in);
        int connectionSocket = accept(this->tcpSocket, (sockaddr*) &clientAddr, &clientAddrSize);
        if(connectionSocket == -1){
            perror("Accept");
            exit(1);
        }
        error = fcntl(connectionSocket, F_SETFL, O_NONBLOCK);
        if(error == -1) {
            perror("Set file descriptor error");
            exit(1);
        }

        std::string request = receiveRequest(connectionSocket);
        std::cout << request << std::endl;

        sendHelloWorld(connectionSocket);

        close(connectionSocket);
    }
    close(this->tcpSocket);
}

std::string Server::receiveRequest(int connectionSocket) const {
    int bytesReceived;
    char buf[255];
    bool isAnyBytesRead = false;
    std::string request;
    while((bytesReceived = (int) recv(connectionSocket, buf, 255, 0)) != -1 || !isAnyBytesRead) {
        if(bytesReceived > 0) isAnyBytesRead = true;
        for(int i = 0; i < bytesReceived; i++) {
            request += buf[i];
        }
    }
    int error = errno;
    if(bytesReceived == -1 && error != EAGAIN && error != EWOULDBLOCK){
        perror("Receive fail");
        exit(1);
    }
    return request;
}

void Server::sendHelloWorld(int connectionSocket) const {
    int error;
    char buf[255] = "Hello World\n\0";
    error = (int) send(connectionSocket, &buf, 255, 0);
    if(error == -1){
        perror("Write");
        exit(1);
    }
}

void Server::setReuseAddr(int sock) {
    const int one = 1;
    int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if(res) error(1, errno, "setsockopt failed");
}
