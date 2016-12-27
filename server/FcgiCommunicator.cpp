#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "FcgiCommunicator.h"
#include "exception/exceptions.h"

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


    char buff[255] = "Hello server\n\0";
    send(tcpSocket, buff, 14, 0);
}

FcgiCommunicator::~FcgiCommunicator() {
    close(tcpSocket);
}

