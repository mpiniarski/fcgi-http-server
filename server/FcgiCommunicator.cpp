#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "FcgiCommunicator.h"
#include "exception/exceptions.h"
#include "../fcgi.h"
#include "Socket.h"

FcgiCommunicator::FcgiCommunicator() {
    tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcpSocket == -1) {
        throw FatalServerException(errno);
    }

    struct sockaddr_in sck_addr;
    sck_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &sck_addr.sin_addr);
    sck_addr.sin_port = htons(8887);
    int socketDescriptor = connect(tcpSocket, (struct sockaddr *) &sck_addr, sizeof sck_addr);
    if (socketDescriptor == -1) {
        throw FatalServerException(errno);
    }
}

void FcgiCommunicator::sendRequest(const std::string &request) const {
    try {
        sendBeginRequest();
        sendContentRequests(request, FCGI_STDIN);
        sendContentRequests("", FCGI_PARAMS);

        Socket socket1 = Socket(tcpSocket);
        std::string response = socket1.receiveMessage();
        std::cout << response;
    }
    catch (ResponseSendException &exception) {
        //TODO log
        throw FatalServerException(errno);
    }
}


void FcgiCommunicator::sendContentRequests(const std::string request, unsigned char type) const {
    unsigned long partSize = 65535;
    for (unsigned long i = 0; i < request.length(); i += partSize) {
        std::string contentData = request.substr(i, partSize);


        FCGI_Header header;
        header.version = FCGI_VERSION_1;
        header.type = type;
        header.requestId = htons(1);
        header.contentLength = htons((uint16_t) contentData.length());
        header.paddingLength = 0;

        ssize_t bytesSent;
        bytesSent = (int) send(tcpSocket, &header, sizeof(header), 0);
        if (bytesSent == -1) {
            throw ResponseSendException(errno);
        }
        bytesSent = (int) send(tcpSocket, contentData.c_str(), contentData.length(), 0);
        if (bytesSent == -1) {
            throw ResponseSendException(errno);
        }
    }
    //End of stream
    FCGI_Header header;
    header.version = FCGI_VERSION_1;
    header.type = type;
    header.requestId = htons(1);
    header.contentLength = 0;
    header.paddingLength = 0;

    ssize_t bytesSent;
    bytesSent = (int) send(tcpSocket, &header, sizeof(header), 0);
    if (bytesSent == -1) {
        throw ResponseSendException(errno);
    }
}

void FcgiCommunicator::sendBeginRequest() const {

    FCGI_BeginRequestBody body;
    body.role = htons(FCGI_RESPONDER);
    body.flags = FCGI_KEEP_CONN;

    FCGI_Header header;
    header.version = FCGI_VERSION_1;
    header.type = FCGI_BEGIN_REQUEST;
    header.requestId = htons(1);
    header.contentLength = htons(sizeof(body));
    header.paddingLength = 0;

    FCGI_BeginRequestRecord beginRecord = {
            header,
            body
    };

    ssize_t bytesSent;
    bytesSent = (int) send(tcpSocket, &beginRecord, sizeof(beginRecord), 0);
    if (bytesSent == -1) {
        throw ResponseSendException(errno);
    }
}

FcgiCommunicator::~FcgiCommunicator() {
    close(tcpSocket);
}

