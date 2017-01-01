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
    void parseRequest(std::string request);

private:
    std::vector<std::string> splitLines(const std::string& header, std::string separator);

};


