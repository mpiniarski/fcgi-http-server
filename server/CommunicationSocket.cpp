#include "CommunicationSocket.h"
#include <unistd.h>
#include <sys/socket.h>

CommunicationSocket::CommunicationSocket(int socketDescriptor) : socketDescriptor(socketDescriptor) {}

CommunicationSocket::~CommunicationSocket() {
    close(socketDescriptor);
}

std::string CommunicationSocket::receiveRequest() const {
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

void CommunicationSocket::sendResponse(std::string response) const {
    ssize_t bytesSent;
    bytesSent = (int) send(socketDescriptor, response.c_str(), response.length(), 0);
    if (bytesSent == -1) {
        throw ResponseSendException(errno);
    }
}
