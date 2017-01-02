#include <cstring>
#include <iostream>
#include <utility>
#include <vector>
#include "FcgiParser.h"


FcgiRequest FcgiParser::parseRequest(std::string request) {
    std::string header = request.substr(0, request.find("\r\n\r\n"));
    std::string body = request.substr(request.find("\r\n\r\n")+4); //+4 to avoid splitter in substring

    std::vector<std::string> lines = split(header, "\r\n");
    std::vector<std::string> firstLineWords = split(lines[0], " ");
    FcgiRequest fcgiRequest;
    splitParameters(lines, fcgiRequest);
    splitParametersFirstLine(firstLineWords, fcgiRequest);
    fcgiRequest.body = body;
    return fcgiRequest;
}

std::vector<std::string> FcgiParser::split(const std::string &string, std::string separator)
{
    std::vector<std::string> lines;
    std::string::size_type start = 0, stop = 0;
    while((stop = string.find(separator, stop)) != std::string::npos)
    {
        std::string line( string.substr(start, stop-start) );
        lines.push_back(line);
        stop += separator.size();
        start = stop;
    }
    lines.push_back(string.substr(start, stop-start));

    return lines;
}

void FcgiParser::splitParameters(std::vector<std::string> lines, FcgiRequest& fcgiRequest) {
    for(int i = 1; i < lines.size(); i++) {
        std::string splitter = ": ";
        std::string key = lines[i].substr(0, lines[i].find(splitter));
        std::string value = lines[i].substr(lines[i].find(splitter)+2);

        fcgiRequest.parameters.insert(std::pair<std::string, std::string>(key, value));
    }
}

void FcgiParser::splitParametersFirstLine(std::vector<std::string> words, FcgiRequest& fcgiRequest) {
    for(int i = 0; i < words.size(); i++) {
        switch(i) {
            case 0:
                fcgiRequest.parameters.insert(std::pair<std::string, std::string>("REQUEST_METHOD", words[i]));
                break;
            case 1:
                splitPath(words[i], fcgiRequest);
                break;
            case 2:
                fcgiRequest.parameters.insert(std::pair<std::string, std::string>("SERVER_PROTOCOL", words[i]));
                break;
            default:
                break;
        }
    }
}

void FcgiParser::splitPath(std::string path, FcgiRequest& fcgiRequest) {
    std::string splitter = "?";
    std::string script_name = path.substr(0, path.find(splitter));
    fcgiRequest.parameters.insert(std::pair<std::string, std::string>("SCRIPT_NAME", script_name));
    std::string query_string = path.substr(path.find(splitter)+1);
    fcgiRequest.parameters.insert(std::pair<std::string, std::string>("QUERY_STRING", query_string));
}

