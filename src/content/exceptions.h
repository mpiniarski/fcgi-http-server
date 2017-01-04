#pragma once

#include "../exceptions.h"
#include "../server/http/http.h"

class UnableToProvideContentException : public Exception {
public:
    UnableToProvideContentException(HttpRequest &request) :
            Exception("Error while providing content for request "
                      + request.method + " " + request.uri) {}
};
