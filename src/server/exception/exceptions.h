#pragma once

#include "../../exceptions.h"
#include "../http/http.h"

class FatalServerException : public Exception {
public:
    FatalServerException(Exception &causedBy) : Exception(
            "Fatal server exception caused by:\n\t" + std::string(causedBy.what())) {}
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

