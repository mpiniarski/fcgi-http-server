#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <zconf.h>
#include <error.h>

class Server {
public:
    Server();
    void listenForever();

private:
    void setReuseAddr(int sock);
    int tcpSocket;
};

