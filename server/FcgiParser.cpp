#include <cstring>
#include <iostream>
#include "FcgiParser.h"


void FcgiParser::parseRequest(std::string request) {
    std::string splitter = "\r\n\r\n";
    std::string header = request.substr(0, request.find(splitter));
    std::string body = request.substr(request.find(splitter)+4); //+4 to avoid splitter in substring

    splitter = "\r\n";
    std::vector<std::string> lines = splitLines(header, splitter);
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

