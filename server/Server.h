#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <zconf.h>
#include <error.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

class Server {
public:
    Server();
    void listenForever();

private:
    int tcpSocket;

    void setReuseAddr(int sock);
    void sendHelloWorld(int connectionSocket) const;
    std::string receiveRequest(int connectionSocket) const;
};

