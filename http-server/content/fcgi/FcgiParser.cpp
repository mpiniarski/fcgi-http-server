#include <cstring>
#include <iostream>
#include <utility>
#include <algorithm>
#include "FcgiParser.h"


FcgiRequest FcgiParser::parseToFcgiRequest(HttpRequest httpRequest) {
    FcgiRequest fcgiRequest;

    fcgiRequest.id = currId++;
    convertHeadersToParameters(httpRequest.headers, fcgiRequest);
    fcgiRequest.parameters.insert(std::pair<std::string, std::string>("REQUEST_METHOD", httpRequest.method));
    fcgiRequest.parameters.insert(std::pair<std::string, std::string>("SERVER_PROTOCOL", httpRequest.version));
    convertPathToParameters(httpRequest.uri, fcgiRequest);
    fcgiRequest.body = httpRequest.body;

    return fcgiRequest;
}

void FcgiParser::convertHeadersToParameters(std::map<std::string,std::string> headers, FcgiRequest &fcgiRequest) {
    for(auto const& iter : headers) {
        std::string key = iter.first;
        std::string value = iter.second;
        if(key != "Content-Type" && key != "Content-Length") {
            key = "HTTP_" + key;
        }
        std::replace(key.begin(), key.end(), '-', '_');
        std::transform(key.begin(), key.end(),key.begin(), ::toupper);
        fcgiRequest.parameters.insert(std::pair<std::string, std::string>(key, value));
    }
}

void FcgiParser::convertPathToParameters(std::string path, FcgiRequest &fcgiRequest) {
    std::string splitter = "?";
    if(path.find(splitter) != std::string::npos) {
        std::string path_info = path.substr(0, path.find(splitter));
        fcgiRequest.parameters.insert(std::pair<std::string, std::string>("PATH_INFO", path_info));
        std::string query_string = path.substr(path.find(splitter) + 1);
        fcgiRequest.parameters.insert(std::pair<std::string, std::string>("QUERY_STRING", query_string));
    }
    else {
        std::string path_info = path;
        fcgiRequest.parameters.insert(std::pair<std::string, std::string>("PATH_INFO", path_info));
    }
}

