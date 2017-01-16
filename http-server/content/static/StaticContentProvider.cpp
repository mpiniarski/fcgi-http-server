#include "StaticContentProvider.h"
#include "../../server/http/HttpParser.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <boost/regex_fwd.hpp>
#include <magic.h>

using namespace boost::filesystem;

std::string StaticContentProvider::getResponse(HttpRequest request) {
    // TODO error responses

    std::string response;
    HttpResponse httpResponse;
    httpResponse.version = HTTP_VERSION_1_0;
    HttpParser httpParser = HttpParser();


    if (request.method != "GET") {
        httpResponse.status = HTTP_400_BAD_REQUEST;
        response = httpParser.parseToStringResponse(httpResponse);
        return response;
    }

    std::string fullPath = getFullPath(request.uri);
    if (exists(fullPath)) {
        if (is_regular_file(fullPath)) {
            // file
            getFileResponse(request.uri, httpResponse);
        }
        else if (is_directory(fullPath)) {
            // directory
            std::string index = fullPath + "index.html";
            if (exists(index)) {
                getDirectoryResponse(index, httpResponse);
            }
            else {
                httpResponse.status = HTTP_404_NOT_FOUND;
                response = httpParser.parseToStringResponse(httpResponse);
                return response;
            }
        }
        else {
            // not fullPath nor directory, but exists
        }
    }
    else {
        // doesn't exist
        httpResponse.status = HTTP_404_NOT_FOUND;
        response = httpParser.parseToStringResponse(httpResponse);
        return response;
    }

    response = httpParser.parseToStringResponse(httpResponse);

    return response;
}

void StaticContentProvider::getFileResponse(std::string filename, HttpResponse &httpResponse) {
    httpResponse.body = getFileContent(getFullPath(filename).c_str());
    httpResponse.status = HTTP_200_OK;
    httpResponse.headers.insert(std::pair<std::string, std::string>("Content-Length", std::to_string(httpResponse.body.length())));
    getFileType(filename, httpResponse);
}

void StaticContentProvider::getDirectoryResponse(std::string index, HttpResponse &httpResponse) {
    httpResponse.body = getFileContent(index.c_str());
    httpResponse.status = HTTP_200_OK;
    httpResponse.headers.insert(std::pair<std::string, std::string>("Content-Length", std::to_string(httpResponse.body.length())));
    getFileType("index.html", httpResponse);
}

std::string StaticContentProvider::getFullPath(std::string uri) {
    return "/home/joanna/http-files/" + getFilename(uri);
}

std::string StaticContentProvider::getFilename(std::string uri) {
    if( uri[0] == '/' ) { uri.erase(0, 1); }
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

void StaticContentProvider::getFileType(std::string uri, HttpResponse &httpResponse) {
    magic_t magicCookie = magic_open(MAGIC_MIME_TYPE);
    magic_load(magicCookie, NULL);
    std::string filename = getFullPath(uri);
    const char *mimetype =  magic_file(magicCookie, filename.c_str());
    httpResponse.headers.insert(std::pair<std::string, std::string>("Content-Type", mimetype));
}


