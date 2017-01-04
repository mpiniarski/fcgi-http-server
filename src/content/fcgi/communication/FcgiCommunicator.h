#pragma once

#include <string>
#include <map>
#include "../../../socket/Socket.h"


struct FcgiResponse{
    std::string STDOUT;
    std::string STDERR;
    uint32_t appStatus;
    uint8_t protocolStatus;
};

struct FcgiRequest {
    std::map<std::string, std::string> parameters;
    std::string body;
};

class FcgiCommunicator {
public:
    FcgiCommunicator();

    void sendRequest(FcgiRequest &request);
    FcgiResponse receiveResponse();

    virtual ~FcgiCommunicator();

private:
    Socket* communicationSocket;

    void sendBeginRecord();

    void sendStream(const std::string request, unsigned char type);

    void sendParameters(const std::map<std::string, std::string> parameters);

    std::string toProperSizeString(uint32_t number);
};


