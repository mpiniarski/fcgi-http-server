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
    void sendRequest(const std::string &request);
    FcgiResponse receiveResponse();

    virtual ~FcgiCommunicator();

private:
    Socket* communicationSocket;

    void sendBeginRecord();

    void sendStream(const std::string request, unsigned char type);

    void sendParameters(const std::map<std::string, std::string> parameters);

    std::string &getString(std::string &contentData, const std::pair<const std::string, std::string> &param);

    std::string toProperSizeString(uint32_t number);
};


