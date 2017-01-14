#include "Socket.h"
#include "../server/exception/exceptions.h"
#include "exceptions.h"
#include <unistd.h>
#include <arpa/inet.h>

Socket::Socket(int socketDescriptor) : socketDescriptor(socketDescriptor) {
    if(socketDescriptor == -1){
        throw SocketCreateException(errno);
    }
}

std::string Socket::receiveMessage() const {
    ssize_t bytesReceived;
    size_t BUFF_SIZE = 255;
    char buf[BUFF_SIZE];
    std::string request;

    bytesReceived = recv(socketDescriptor, buf, BUFF_SIZE, 0);
    if (bytesReceived > 0) {
        request.append(buf, (unsigned long) bytesReceived);
        if (bytesReceived == BUFF_SIZE) {
            while (((bytesReceived = recv(socketDescriptor, buf, BUFF_SIZE, MSG_DONTWAIT)) > 0)) {
                for (int i = 0; i < bytesReceived; i++) {
                    request += buf[i];
                }
            }
            if (bytesReceived == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
                throw SocketMessageReceiveException(errno);
            }
        }
    } else {
        throw SocketMessageReceiveException(errno);
    }
    return request;
}

std::string Socket::receiveMessage(size_t size) const {
    std::string message;
    while(size > 0){
        ssize_t bytesReceived;
        char buf[size];
        bytesReceived = recv(socketDescriptor, buf, size, 0);
        if (bytesReceived < 0){
            throw SocketMessageReceiveException(errno);
        }
        message.append(buf, (unsigned long) bytesReceived);
        size -= bytesReceived;
    }
    return message;
}


void Socket::sendMessage(std::string response) const {
    ssize_t bytesSent;
    bytesSent = (int) send(socketDescriptor, response.c_str(), response.length(), 0);
    if (bytesSent == -1) {
        throw SocketMessageSendException(errno);
    }
}

void Socket::sendMessage(const void *buff, size_t size) const {
    ssize_t bytesSent;
    bytesSent = (int) send(socketDescriptor, buff, size, 0);
    if (bytesSent == -1) {
        throw SocketMessageSendException(errno);
    }
}

void Socket::setReuseAddr() {
    const int flagValue = 1;
    int result = setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &flagValue, sizeof(flagValue));
    if (result == -1) {
        throw SocketOptionsChangeException(errno);
    }
}

void Socket::bindTo(std::string& address, uint16_t port) {
    int error;
    struct sockaddr_in myAddr;
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(port);
    error = inet_aton(address.c_str(), &myAddr.sin_addr);
    if(error == -1){
        throw IpAddressException(address, errno);
    }
    error = bind(socketDescriptor, (sockaddr *) &myAddr, sizeof myAddr);
    if (error == -1) {
        throw SocketBindException(address, port, errno);
    }
}

void Socket::setListen(int queueSize) {
    int result = listen(socketDescriptor, queueSize);
    if (result == -1) {
        throw SocketListenException(queueSize, errno);
    }
}

Socket* Socket::acceptConnection() {
//    struct sockaddr_in clientAddr;
//    socklen_t clientAddrSize = sizeof(sockaddr_in);
    return acceptConnection(NULL, NULL);
}

Socket* Socket::acceptConnection(sockaddr_in *addrPtr, socklen_t *addrLenPtr) {
    int acceptedSocketDescriptor = accept(socketDescriptor, (sockaddr *) addrPtr, addrLenPtr);
    if (acceptedSocketDescriptor == -1) {
        throw SocketAcceptException(errno);
    }
    return new Socket(acceptedSocketDescriptor);
}

void Socket::connectWith(std::string address, uint16_t port) {
    int error;
    struct sockaddr_in sck_addr;
    sck_addr.sin_family = AF_INET;
    inet_aton(address.c_str(), &sck_addr.sin_addr);
    if(error == -1){
        throw IpAddressException(address, errno);
    }
    sck_addr.sin_port = htons(port);
    error = connect(socketDescriptor, (struct sockaddr *) &sck_addr, sizeof sck_addr);
    if (error == -1) {
        throw SocketConnectException(address, port, errno);
    }
}

Socket::~Socket() {
    close(socketDescriptor);
}
