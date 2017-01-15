#pragma once

#include "../../exceptions.h"

class HttpParserException : public Exception {
public:
    HttpParserException(const std::string &message) : Exception(message) {}

    HttpParserException(const std::string &message, Exception &parent) : Exception(message, parent) {}
};

class HttpParserBadSyntaxException : public HttpParserException {

public:
    HttpParserBadSyntaxException() : HttpParserException(getMessage()) {}

    HttpParserBadSyntaxException(Exception &parent) : HttpParserException(getMessage(), parent) {}

private:
    std::string getMessage() {
        return "Bad syntax of HTTP request";
    }

};
