#pragma once

#include "../../exceptions.h"
#include "../http/http.h"

class FatalServerException : public Exception {
public:
    FatalServerException() : Exception(getMessage()) {};
    FatalServerException(Exception &causedBy) : Exception(getMessage(), causedBy) {};
private:
    std::string getMessage() {
        return "Fatal server exception";
    }
};
