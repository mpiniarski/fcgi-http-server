#pragma once

#include "../server/http/http.h"

class ContentProvider {
public:
    virtual HttpResponse getResponse(HttpRequest request) = 0;
};




