#pragma once

#include "http.h"

class HttpParser {
public:
    HttpRequest parseToHttpRequest(std::string request);
    std::string parseToStringResponse(HttpResponse httpResponse);
};


