#include <vector>
#include <algorithm>
#include <regex>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "HttpParser.h"
#include "exceptions.h"

HttpRequest HttpParser::parseToHttpRequest(std::string request) {
    HttpRequest httpRequest;
    std::string header;
    if(request.find("\r\n\r\n") != std::string::npos) {
        header = request.substr(0, request.find("\r\n\r\n"));
        httpRequest.body = request.substr(request.find("\r\n\r\n") + 4);
    }
    else { header = request; }

    std::vector<std::string> lines = split(header, "\r\n");
    httpRequest.headers = convertHeadersToMap(lines);
    std::string firstLine = lines[0];
    std::vector<std::string> words = split(firstLine, " ");
    if(words.size() != 3) {
        throw HttpParserBadSyntaxException();
    }
    else {
        httpRequest.method = words[0];
        httpRequest.uri = words[1];
        httpRequest.version = words[2];
    }
    validateRequestSyntax(httpRequest);

    return httpRequest;
}

std::vector<std::string> HttpParser::split(const std::string &string, std::string separator) {
    std::vector<std::string> strings;
    std::string::size_type start = 0, stop = 0;
    while ((stop = string.find(separator, stop)) != std::string::npos) {
        std::string line(string.substr(start, stop - start));
        if(line.length() > 0) {
            strings.push_back(line);
        }
        stop += separator.size();
        start = stop;
    }
    std::string line(string.substr(start, stop - start));
    if(line.length() > 0) {
        strings.push_back(line);
    }

    return strings;
}

std::map<std::string, std::string> HttpParser::convertHeadersToMap(std::vector<std::string> headers) {
    std::map<std::string, std::string> headersMap;

    std::vector<std::string> headersNonMultiline;
    for (int i = 1; i < headers.size(); i++) {
        if(!isspace(headers[i][0])) {
            headersNonMultiline.push_back(headers[i]);
        }
        else {
            std::string lastBunOne = headersNonMultiline.back();
            headersNonMultiline.pop_back();
            std::string current = boost::trim_copy(headers[i]);
            headersNonMultiline.push_back(lastBunOne + " " + current);
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

std::string HttpParser::parseToStringResponse(HttpResponse httpResponse) {
    std::string lineSeparator = "\r\n";
    std::string response;
    response += httpResponse.version + " "
                    + std::to_string(httpResponse.status.code) + " "
                    + httpResponse.status.message + lineSeparator;
    for(auto header : httpResponse.headers){
        response += header.first + ": " + header.second + lineSeparator;
    }
    response += lineSeparator + httpResponse.body;
    return response;
}

void HttpParser::validateRequestSyntax(HttpRequest httpRequest) {
    if (httpRequest.method != "GET" && httpRequest.method != "POST" && httpRequest.method != "HEAD") {
        throw HttpParserBadSyntaxException();
    }

    std::regex pathRegex("\\/([\\w\\.-]+\\/?)*(\\?([\\w\\.-]+\\=[\\w\\.-]+(&|;))*([\\w\\.-]+\\=[\\w\\.-]+))?");
    if (!std::regex_match(httpRequest.uri, pathRegex)) {
        throw HttpParserBadSyntaxException();
    }

    std::regex versionRegex("HTTP/1\\.(1|0)");
    if (!std::regex_match(httpRequest.version, versionRegex)) {
        throw HttpParserBadSyntaxException();
    }
}

