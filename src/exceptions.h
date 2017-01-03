#pragma once

#include <exception>
#include <string>
#include <cstring>

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
