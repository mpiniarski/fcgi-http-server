#include <string>
#pragma once

class FcgiCommunicator {
public:
    FcgiCommunicator();
    void sendRequest(const std::string &request) const;
    virtual ~FcgiCommunicator();

private:
    int tcpSocket;

    void sendBeginRequest() const;
    void sendContentRequests(const std::string request) const;
    void sendStreamEndRequest() const;
};

