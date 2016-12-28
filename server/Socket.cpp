#include "Socket.h"
#include <unistd.h>
#include <arpa/inet.h>

Socket::Socket(int socketDescriptor) : socketDescriptor(socketDescriptor) {}

std::string Socket::receiveMessage() const {
    ssize_t bytesReceived;
    size_t BUFF_SIZE = 255;
    char buf[BUFF_SIZE];
    std::string request;

    bytesReceived = recv(socketDescriptor, buf, BUFF_SIZE, 0);
    if (bytesReceived > 0) {
        for (int i = 0; i < bytesReceived; i++) {
            request += buf[i];
        }

        if (bytesReceived == BUFF_SIZE) {
            while (((bytesReceived = recv(socketDescriptor, buf, BUFF_SIZE, MSG_DONTWAIT)) > 0)) {
                for (int i = 0; i < bytesReceived; i++) {
                    request += buf[i];
                }
            }
            if (bytesReceived == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
                throw RequestReceiveException(errno);
            }
        }
    } else {
        throw RequestReceiveException(errno);
    }
    return request;
}

void Socket::sendMessage(std::string response) const {
    ssize_t bytesSent;
    bytesSent = (int) send(socketDescriptor, response.c_str(), response.length(), 0);
    if (bytesSent == -1) {
        throw ResponseSendException(errno);
    }
}

void Socket::sendMessage(const void *buff, size_t size) const {
    ssize_t bytesSent;
    bytesSent = (int) send(socketDescriptor, buff, size, 0);
    if (bytesSent == -1) {
        throw ResponseSendException(errno);
    }
}

void Socket::setReuseAddr() {
    const int flagValue = 1;
    int result = setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &flagValue, sizeof(flagValue));
    if (result == -1) {
        throw FatalServerException(errno);
    }
}

void Socket::bindToLocalhost(uint16_t port) {
    struct sockaddr_in myAddr;
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(port);
    myAddr.sin_addr.s_addr = INADDR_ANY;
    int result = bind(socketDescriptor, (sockaddr *) &myAddr, sizeof myAddr);
    if (result == -1) {
        throw FatalServerException(errno);
    }

}

void Socket::setListen(int queueSize) {
    int result = listen(socketDescriptor, queueSize);
    if (result == -1) {
        throw FatalServerException(errno);
    }
}

Socket Socket::acceptConnection() {
//    struct sockaddr_in clientAddr;
//    socklen_t clientAddrSize = sizeof(sockaddr_in);
    return acceptConnection(NULL, NULL);
}

Socket Socket::acceptConnection(sockaddr_in *addrPtr, socklen_t *addrLenPtr) {
    int acceptedSocketDescriptor = accept(socketDescriptor, (sockaddr *) addrPtr, addrLenPtr);
    if (acceptedSocketDescriptor == -1) {
        throw FatalServerException(errno);
    }
    return Socket(acceptedSocketDescriptor);
}

Socket::~Socket() {
    close(socketDescriptor);
}
