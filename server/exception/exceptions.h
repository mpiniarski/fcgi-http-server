#pragma once

#include <exception>
#include <memory.h>

#include <string>
#include <stdexcept>


class ErrorCodeBasedException : public std::exception {
protected:
    ErrorCodeBasedException(std::string title, int errorNumber) : std::exception() {
        message = title + ": " + strerror(errorNumber);
    }

public:
    virtual const char *what() {
        return message.c_str();
    }

private:
    std::string message;
};

class FatalServerException : public ErrorCodeBasedException {
public:
    FatalServerException(int errorNumber) : ErrorCodeBasedException("Fatal server exception", errorNumber) {}
};

class RequestReceiveException : public ErrorCodeBasedException {
public:
    RequestReceiveException(int errorNumber) : ErrorCodeBasedException("Error while receiving request", errorNumber) {}
};

class ResponseSendException : public ErrorCodeBasedException {
public:
    ResponseSendException(int errorNumber) : ErrorCodeBasedException("Error while sending response", errorNumber) {}
};


class HttpException : public std::exception {
public:
    HttpException(int status) : std::exception(), status(status) {}

    int getStatus() const {
        return status;
    }

private:
    int status;
};

class InternalServerException : public HttpException {
public:
    InternalServerException() : HttpException(500) {}
};
