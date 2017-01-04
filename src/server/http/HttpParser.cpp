#include <vector>
#include <algorithm>
#include "HttpParser.h"

HttpRequest HttpParser::parseToHttpRequest(std::string request) {
    HttpRequest httpRequest;
    std::string header = request.substr(0, request.find("\r\n\r\n"));
    httpRequest.body = request.substr(request.find("\r\n\r\n") + 4);

    std::vector<std::string> lines = split(header, "\r\n");
    httpRequest.headers = convertHeadersToMap(lines);
    std::string firstLine = lines[0];
    std::vector<std::string> words = split(firstLine, " ");
    httpRequest.method = words[0];
    httpRequest.uri = words[1];
    httpRequest.version = words[2];

    return httpRequest;
}

std::vector<std::string> HttpParser::split(const std::string &string, std::string separator) {
    std::vector<std::string> strings;
    std::string::size_type start = 0, stop = 0;
    while ((stop = string.find(separator, stop)) != std::string::npos) {
        std::string line(string.substr(start, stop - start));
        strings.push_back(line);
        stop += separator.size();
        start = stop;
    }
    strings.push_back(string.substr(start, stop - start));

    return strings;
}

std::map<std::string, std::string> HttpParser::convertHeadersToMap(std::vector<std::string> headers) {
    std::map<std::string, std::string> headersMap;

    std::vector<std::string> headersNonMultiline;
    //TODO multiline
    for (int i = 1; i < headers.size(); i++) {
        if(!isspace(headers[i][0])) {
            headersNonMultiline.push_back(headers[i]);
        }
        else {
            headersNonMultiline.push_back(headers[i-1] + " " + headers[i]);    //TODO trim whitespace
        }
    }

    for (int i = 0; i < headersNonMultiline.size(); i++) {
        std::string splitter = ": ";
        std::string key = headersNonMultiline[i].substr(0, headersNonMultiline[i].find(splitter));
        std::string value = headersNonMultiline[i].substr(headersNonMultiline[i].find(splitter) + 2);

        headersMap.insert(std::pair<std::string, std::string>(key, value));
    }
    return headersMap;
}

