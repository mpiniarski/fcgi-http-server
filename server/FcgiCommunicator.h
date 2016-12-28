#include <string>
#pragma once

class FcgiCommunicator {
public:
    FcgiCommunicator();
    virtual ~FcgiCommunicator();

private:
    int tcpSocket;

    void sendBeginRequest() const;
};

