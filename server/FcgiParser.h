#include <string>
#include <map>

#pragma once

struct FcgiRequest {
    std::map<std::string, std::string> parameters;
    std::string body;
};

class FcgiParser {
public:
    void parseRequest(std::string request);

private:
    void parseInitialLine(std::string line);

};


