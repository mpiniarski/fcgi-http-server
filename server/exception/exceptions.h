#pragma once

#include <exception>

class HttpException : public std::exception {
public:
    HttpException(int status) : std::exception() {}
private:
    int status;
};

class InternalServerException : public HttpException {
public:
    InternalServerException() : HttpException(500) {}
};
