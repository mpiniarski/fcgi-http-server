#include <cstring>
#include <iostream>
#include <utility>
#include <vector>
#include "FcgiParser.h"


void FcgiParser::parseRequest(std::string request) {
    std::string splitter = "\r\n\r\n";
    std::string header = request.substr(0, request.find(splitter));
    std::string body = request.substr(request.find(splitter)+4); //+4 to avoid splitter in substring

    splitter = "\r\n";
    std::vector<std::string> lines = splitLines(header, splitter);
    FcgiRequest FcgiRequest = splitParameters(lines);
}

std::vector<std::string> FcgiParser::splitLines(const std::string& header, std::string separator)
{
    std::vector<std::string> lines;
    std::string::size_type start = 0, stop = 0;
    while((stop = header.find(separator, stop)) != std::string::npos)
    {
        std::string line( header.substr(start, stop-start) );
        lines.push_back(line);
        stop += 2;
        start = stop;
    }
    lines.push_back(header.substr(start, stop-start));

    return lines;
}

FcgiRequest FcgiParser::splitParameters(std::vector<std::string> lines) {
    FcgiRequest FcgiRequest;
    for(int i = 1; i < lines.size(); i++) {
        std::string splitter = ": ";
        std::string key = lines[i].substr(0, lines[i].find(splitter));
        std::string value = lines[i].substr(lines[i].find(splitter)+2);

        FcgiRequest.parameters.insert(std::pair<std::string, std::string>(key, value));
    }
    return FcgiRequest;
}

