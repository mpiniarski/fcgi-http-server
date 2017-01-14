#pragma once

#include "../server/exception/exceptions.h"

class SocketException : public ErrorCodeBasedException {
public:
    SocketException(const std::string &comment, int errorNumber) : ErrorCodeBasedException(comment, errorNumber) {}
};

class SocketCreateException : public SocketException {
public:
    SocketCreateException(int errorNumber) : SocketException("Error while creating socket", errorNumber) {}
};

class SocketMessageReceiveException : public SocketException {
public:
    SocketMessageReceiveException(int errorNumber) : SocketException("Error while receiving message",
                                                                     errorNumber) {}
};

class SocketMessageSendException : public SocketException {
public:
    SocketMessageSendException(int errorNumber) : SocketException("Error while sending message", errorNumber) {}
};

class SocketOptionsChangeException : public SocketException {
public:
    SocketOptionsChangeException(int errorNumber) : SocketException("Error while changing socket options",
                                                                    errorNumber) {}
};

class SocketBindException : public SocketException {
public:
    SocketBindException(std::string &ip, uint16_t port, int errorNumber) : SocketException(
            "Error while binding to " + ip + ":" + std::to_string(port), errorNumber) {}
};

class IpAddressException : public SocketException {
public:
    IpAddressException(std::string &ip, int errorNumber) : SocketException(
            "Ip address is not valid" + ip, errorNumber) {}
};

class SocketListenException : public SocketException {
public:
    SocketListenException(int queueSize, int errorNumber) : SocketException(
            "Error while setting socket to listen mode with queue size " + std::to_string(queueSize),
            errorNumber) {}
};

class SocketAcceptException : public SocketException {
public:
    SocketAcceptException(int errorNumber) : SocketException("Error while accepting connection", errorNumber) {}
};

class SocketConnectException : public SocketException {
public:
    SocketConnectException(std::string address, uint16_t port, int errorNumber) : SocketException(
            std::string("Error while connecting to ") + address + ":" + std::to_string(port), errorNumber) {}

};
