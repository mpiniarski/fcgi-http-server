#pragma once

#include "exception/exceptions.h"
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Socket {
public:
    Socket(int socketDescriptor);

    void sendMessage(std::string response) const;

    std::string receiveMessage() const;

    void setReuseAddr();

    void bindToLocalhost(uint16_t port);

    void setListen(int queueSize);

    Socket acceptConnection();

    Socket acceptConnection(sockaddr_in *addrPtr, socklen_t *addrLenPtr);

    virtual ~Socket();

private:
    int socketDescriptor;

    void sendMessage(const void *buff, size_t size) const;

};


