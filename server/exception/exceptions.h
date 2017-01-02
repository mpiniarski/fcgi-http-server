#pragma once

#include <exception>
#include <memory.h>

#include <string>
#include <stdexcept>
#include "../HttpResponder.h"

class Exception : public std::exception {
public:
    Exception(const std::string &message) : message(message) {}

    virtual const char *what() {
        return message.c_str();
    }

private:
    std::string message;
};


class ErrorCodeBasedException : public Exception {
protected:
    ErrorCodeBasedException(std::string comment, int errorNumber) : Exception(
            comment + ": " + strerror(errorNumber)) {}
};

class FatalServerException : public Exception {
public:
    FatalServerException(Exception &causedBy) : Exception(
            "Fatal server exception caused by:\n\t" + std::string(causedBy.what())) {}
};

class SocketException : public ErrorCodeBasedException {
public:
    SocketException(const std::string &comment, int errorNumber) : ErrorCodeBasedException(comment, errorNumber) {}
};

class SocketCreateException : public SocketException {
public:
    SocketCreateException(int errorNumber) : SocketException("Error while creating socket", errorNumber) {}
};

class SocketRequestReceiveException : public SocketException {
public:
    SocketRequestReceiveException(int errorNumber) : SocketException("Error while receiving request",
                                                                     errorNumber) {}
};

class SocketResponseSendException : public SocketException {
public:
    SocketResponseSendException(int errorNumber) : SocketException("Error while sending response", errorNumber) {}
};

class SocketOptionsChangeException : public SocketException {
public:
    SocketOptionsChangeException(int errorNumber) : SocketException("Error while changing socket options",
                                                                    errorNumber) {}
};

class SocketBindException : public SocketException {
public:
    SocketBindException(uint16_t port, int errorNumber) : SocketException(
            "Error while binding to port " + std::to_string(port), errorNumber) {}
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


class HttpException : public std::exception {
public:
    HttpException(const HttpStatus &status) : status(status) {}

    const HttpStatus &getStatus() const {
        return status;
    }

private:
    HttpStatus status;
};

