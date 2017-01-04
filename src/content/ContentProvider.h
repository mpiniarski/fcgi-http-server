#pragma once

#include "../server/http/http.h"

class ContentProvider {
public:
    virtual std::string getResponse(HttpRequest request) = 0;
};
