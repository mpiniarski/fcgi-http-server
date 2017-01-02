#include <string>
#include <map>
#include <vector>

#pragma once

struct FcgiRequest {
    std::map<std::string, std::string> parameters;
    std::string body;
};

class FcgiParser {
public:
    FcgiRequest parseRequest(std::string request);

private:
    std::vector<std::string> split(const std::string &string, std::string separator);

    void convertHeadersToParameters(std::vector<std::string> lines, FcgiRequest &fcgiRequest);

    void convertFirstLineToParameters(std::string words, FcgiRequest &fcgiRequest);

    void convertPathToParameters(std::string path, FcgiRequest &fcgiRequest);
};


