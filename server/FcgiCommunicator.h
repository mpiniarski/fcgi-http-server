#include <string>
#include "Socket.h"
#include <map>

#pragma once

class FcgiCommunicator {
public:
    FcgiCommunicator();
    void sendRequest(const std::string &request) const;

    virtual ~FcgiCommunicator();

private:
    Socket* communicationSocket;

    void sendBeginRecord() const;

    void sendStream(const std::string request, unsigned char type) const;

    void sendParameters(const std::map<std::string, std::string> parameters) const;
};

