#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>

class Socket {
public:
    Socket(int socketDescriptor);

    void sendMessage(std::string response) const;

    void sendMessage(const void *buff, size_t size) const;

    std::string receiveMessage() const;

    std::string receiveMessage(size_t size) const;

    void setReuseAddr();

    void bindTo(std::string &address, uint16_t port);

    void setListen(int queueSize);

    Socket * acceptConnection();

    Socket * acceptConnection(sockaddr_in *addrPtr, socklen_t *addrLenPtr);

    void connectWith(std::string address, uint16_t port);

    virtual ~Socket();

private:
    int socketDescriptor;

};


