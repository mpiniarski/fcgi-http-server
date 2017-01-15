#include "StaticContentProvider.h"
#include "../../server/http/HttpParser.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <boost/regex_fwd.hpp>

using namespace boost::filesystem;

std::string StaticContentProvider::getResponse(HttpRequest request) {
    // TODO error responses

    std::string response;
    HttpResponse httpResponse;
    httpResponse.version = HTTP_VERSION_1_0;

    if (request.method != "GET") {
        response = "Not a get method.";
        return response;
    }

    std::string path = getFullPath(request.uri);

    if (exists(path)) {
        if (is_regular_file(path)) {
            // file
            getFileResponse(path, httpResponse);
        }
        else if (is_directory(path)) {
            // directory
        }
        else {
            // not path nor directory, but exists
        }
    }
    else {
        // doesn't exist
        response = "File does not exist.";
    }

    HttpParser httpParser = HttpParser();
    response = httpParser.parseToStringResponse(httpResponse);

    return response;
}

void StaticContentProvider::getFileResponse(std::string path, HttpResponse &httpResponse) {
    const char* filename = path.c_str();
    httpResponse.body = getFileContent(filename);
    httpResponse.status = HTTP_200_OK;
    std::map<std::string, std::string> headers;
    headers.insert(std::pair<std::string, std::string>("Content-Length", std::to_string(httpResponse.body.length())));
    headers.insert(std::pair<std::string, std::string>("Content-Type", "text/html"));
    httpResponse.headers = headers;
}

std::string StaticContentProvider::getFullPath(std::string uri) {
    if( uri[0] == '/' ) { uri.erase(0, 1); }
    uri = "/home/joanna/http-files/" + uri;
    return uri;
}

std::string StaticContentProvider::getFileContent(const char *filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}



