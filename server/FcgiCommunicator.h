#include <string>
#include "Socket.h"
#include <map>

#pragma once

struct FcgiResponse{
    std::string STDOUT;
    std::string STDERR;
    uint32_t appStatus;
    uint8_t protocolStatus;
};

class FcgiCommunicator {
public:
    FcgiCommunicator();
    void sendRequest(const std::string &request) const;
    FcgiResponse receiveResponse();

    virtual ~FcgiCommunicator();

private:
    Socket* communicationSocket;

    void sendBeginRecord() const;

    void sendStream(const std::string request, unsigned char type) const;

    void sendParameters(const std::map<std::string, std::string> parameters) const;
};


