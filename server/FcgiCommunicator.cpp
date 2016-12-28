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

    try {
        sendBeginRequest();
    }
    catch(ResponseSendException &exception) {
        //TODO log
        throw FatalServerException(errno);
    }
}

void FcgiCommunicator::sendBeginRequest() const {

    FCGI_BeginRequestBody body;
    body.roleB1 = 0;
    body.roleB0 = FCGI_RESPONDER;
    body.flags = FCGI_KEEP_CONN;

    FCGI_Header header;
    header.version = FCGI_VERSION_1;
    header.type = FCGI_BEGIN_REQUEST;
    header.requestIdB1 = 0;
    header.requestIdB0 = FCGI_NULL_REQUEST_ID;
    header.contentLengthB1 = 0;
    header.contentLengthB0 = sizeof(body);
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

