#pragma once

#include "http.h"
#include <vector>

class HttpParser {
public:
    HttpRequest parseToHttpRequest(std::string request);
    std::string parseToStringResponse(HttpResponse httpResponse);

private:
    std::vector<std::string> split(const std::string &string, std::string separator);
    std::map<std::string,std::string> convertHeadersToMap(std::vector<std::string> headers);
};

