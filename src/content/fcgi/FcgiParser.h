#pragma once
#include <string>
#include <map>
#include <vector>
#include "communication/FcgiCommunicator.h"


class FcgiParser {
public:
    FcgiRequest parseRequest(std::string request);

private:
    std::vector<std::string> split(const std::string &string, std::string separator);

    void convertHeadersToParameters(std::vector<std::string> lines, FcgiRequest &fcgiRequest);

    void convertFirstLineToParameters(std::string words, FcgiRequest &fcgiRequest);

    void convertPathToParameters(std::string path, FcgiRequest &fcgiRequest);
};


