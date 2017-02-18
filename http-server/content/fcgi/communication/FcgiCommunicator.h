#pragma once

#include <string>
#include <map>
#include "../../../socket/Socket.h"
#include "../../../config/ConfigProvider.h"
#include <thread>
#include <condition_variable>


struct FcgiResponse{
    std::string STDOUT;
    std::string STDERR;
    uint32_t appStatus;
    uint8_t protocolStatus;
    bool isFinished;
};

struct FcgiRequest {
    uint16_t id;
    std::map<std::string, std::string> parameters;
    std::string body;
};

class FcgiCommunicator {
public:
    FcgiCommunicator(HostAddress& fcgiAddress);

    void sendRequest(FcgiRequest &request);
    FcgiResponse receiveResponse(int requestId);

    virtual ~FcgiCommunicator();

private:
    Socket* communicationSocket;
    bool isListening;
    std::map<uint16_t, FcgiResponse> responseMap;
    std::thread listener;
    std::mutex mutex;
    std::condition_variable conditionalVariable;

    void sendBeginRecord(uint16_t requestId);
    void sendStream(uint16_t requestId, const std::string content, unsigned char type);
    void sendParameters(uint16_t requestId, const std::map<std::string, std::string> parameters);
    std::string toProperSizeString(uint32_t number);


    void listenForResponses();
    void manageResponse();
};


