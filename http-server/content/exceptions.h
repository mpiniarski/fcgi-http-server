#pragma once

#include "../exceptions.h"
#include "../server/http/http.h"

class ContentProviderRespondingException : public Exception {
public:
    ContentProviderRespondingException(HttpRequest &request) : Exception(getMessage(request)) {}

    ContentProviderRespondingException(HttpRequest &request, Exception &exception) :
            Exception(getMessage(request), exception) {}

private:
    std::string getMessage(HttpRequest &request) {
        return "Error while providing content for request "
               + request.method + " " + request.uri;
    }
};

class ContentProviderCreatingException : public Exception {
public:
    ContentProviderCreatingException() : Exception(getMessage()) {}

    ContentProviderCreatingException(Exception &exception) : Exception(getMessage(), exception) {}

private:
    std::string getMessage() {
        return "Error while creating content provider";
    }

};
