#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "FcgiComunicator.h"

FcgiComunicator::FcgiComunicator() {
    this->tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(this->tcpSocket == -1){
        perror("Socket fail - fcgi comunicator");
        exit(1);
    }

    struct sockaddr_in sck_addr;
    sck_addr.sin_family = AF_INET;
    inet_aton ("127.0.0.1", &sck_addr.sin_addr);
    sck_addr.sin_port = htons(8889);
    int error = connect(this->tcpSocket, (struct sockaddr*) &sck_addr, sizeof sck_addr);
    if(error == -1){
        perror("Connect fail - fcgi comunicator");
        exit(1);
    }
    char buff[255] = "Hello server\n\0";
    send(tcpSocket, buff, 14, 0);
}

FcgiComunicator::~FcgiComunicator() {
    close(this->tcpSocket);
}

