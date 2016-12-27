#pragma once

#include "exception/exceptions.h"
#include <string>

class CommunicationSocket {
public:
    CommunicationSocket(int socketDescriptor);

    void sendResponse(std::string response) const;

    std::string receiveRequest() const;

    virtual ~CommunicationSocket();

private:
    int socketDescriptor;

};


