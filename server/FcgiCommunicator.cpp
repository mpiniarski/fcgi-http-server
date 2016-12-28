#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include "FcgiCommunicator.h"
#include "exception/exceptions.h"
#include "../fcgi.h"

FcgiCommunicator::FcgiCommunicator() {
    tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(tcpSocket == -1){
        throw FatalServerException(errno);
    }

    struct sockaddr_in sck_addr;
    sck_addr.sin_family = AF_INET;
    inet_aton ("127.0.0.1", &sck_addr.sin_addr);
    sck_addr.sin_port = htons(8889);
    int socketDescriptor = connect(tcpSocket, (struct sockaddr*) &sck_addr, sizeof sck_addr);
    if(socketDescriptor == -1){
        throw FatalServerException(errno);
    }
}

void FcgiCommunicator::sendRequest(const std::string &request) const {
    try {
        sendBeginRequest();
        sendContentRequest(request);
        sendStreamEndRequest();
    }
    catch(ResponseSendException &exception) {
        //TODO log
        throw FatalServerException(errno);
    }
}

void FcgiCommunicator::sendStreamEndRequest() const { sendContentRequest(""); }

void FcgiCommunicator::sendContentRequest(const std::string request) const {
    FCGI_RequestBody body;
    body.contentData = request;

    FCGI_Header header;
    header.version = FCGI_VERSION_1;
    header.type = FCGI_BEGIN_REQUEST;
    header.requestId = FCGI_NULL_REQUEST_ID;
    header.contentLength = body.contentData.length();
    header.paddingLength = 0;

    FCGI_RequestRecord record = {
            header,
            body
    };

    ssize_t bytesSent;
    bytesSent = (int) send(tcpSocket, &record, sizeof(record), 0);
    if (bytesSent == -1) {
        throw ResponseSendException(errno);
    }
}

void FcgiCommunicator::sendBeginRequest() const {

    FCGI_BeginRequestBody body;
    body.role = FCGI_RESPONDER;
    body.flags = FCGI_KEEP_CONN;

    FCGI_Header header;
    header.version = FCGI_VERSION_1;
    header.type = FCGI_BEGIN_REQUEST;
    header.requestId = FCGI_NULL_REQUEST_ID;
    header.contentLength = sizeof(body);
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

