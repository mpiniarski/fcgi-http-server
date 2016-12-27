#include <stdlib.h>
#include <cstring>
#include "Server.h"
#include "exception/exceptions.h"


class ErrorCodeBasedException : public std::exception {
public:
    ErrorCodeBasedException(int errorNumber) : std::exception() {
        this->errorNumber = errorNumber;
    }

    const char *what() const throw() {
        return strerror(errorNumber);
    }

private:
    int errorNumber;
};

class RequestReceiveException : public ErrorCodeBasedException {
public:
    RequestReceiveException(int errorNumber) : ErrorCodeBasedException(errorNumber) {}
};

class ResponseSendException : public ErrorCodeBasedException {
public:
    ResponseSendException(int errorNumber) : ErrorCodeBasedException(errorNumber) {}
};


Server::Server() {
    int error;

    this->tcpSocket = socket(PF_INET, SOCK_STREAM, 0);
    setReuseAddr(this->tcpSocket);

    struct sockaddr_in myAddr;
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(8888);
    myAddr.sin_addr.s_addr = INADDR_ANY;

    error = bind(this->tcpSocket, (sockaddr *) &myAddr, sizeof myAddr);
    if (error == -1) {
        //TODO log
        throw new InternalServerException();
    }

    int connectionSockets = 1;
    error = listen(this->tcpSocket, connectionSockets);
    if (error == -1) {
        //TODO log
        throw new InternalServerException();
    }
}

void Server::listenForever() {
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(sockaddr_in);
        int connectionSocket = accept(this->tcpSocket, (sockaddr *) &clientAddr, &clientAddrSize);
        if (connectionSocket == -1) {
            //TODO log
            throw new InternalServerException();
        }

        try {
            std::string request = receiveRequest(connectionSocket);
            std::cout << request << std::endl;

            sendHelloWorld(connectionSocket);
        }
        catch (RequestReceiveException exception) {
            //TODO log
            throw new InternalServerException();
        }
        catch (ResponseSendException exception){
            //TODO log
            throw new InternalServerException();
        }

        close(connectionSocket);
    }
    close(this->tcpSocket);
}

std::string Server::receiveRequest(int connectionSocket) const {
    ssize_t bytesReceived;
    size_t BUFF_SIZE = 255;
    char buf[BUFF_SIZE];
    std::string request;

    bytesReceived = recv(connectionSocket, buf, BUFF_SIZE, 0);
    if (bytesReceived > 0) {
        for (int i = 0; i < bytesReceived; i++) {
            request += buf[i];
        }

        if (bytesReceived == BUFF_SIZE) {
            while (((bytesReceived = recv(connectionSocket, buf, BUFF_SIZE, MSG_DONTWAIT)) > 0)) {
                for (int i = 0; i < bytesReceived; i++) {
                    request += buf[i];
                }
            }
            if (bytesReceived == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
                throw new RequestReceiveException(errno);
            }
        }
    } else {
        throw new RequestReceiveException(errno);
    }
    return request;
}

void Server::sendHelloWorld(int connectionSocket) const {
    int error;
    char buf[255] = "Hello World\n\0";
    error = (int) send(connectionSocket, &buf, 255, 0);
    if (error == -1) {
        throw new ResponseSendException(errno);
    }
}

void Server::setReuseAddr(int sock) {
    const int one = 1;
    int res = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    if (res) error(1, errno, "setsockopt failed");
}
