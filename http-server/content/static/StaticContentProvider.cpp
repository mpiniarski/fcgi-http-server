#include "StaticContentProvider.h"
#include "../exceptions.h"
#include "exceptions.h"
#include <boost/filesystem.hpp>
#include <iostream>
#include <boost/regex_fwd.hpp>
#include <magic.h>

using namespace boost::filesystem;

StaticContentProvider::StaticContentProvider(std::string rootPath) {
    this->httpParser = new HttpParser();
    this->rootPath = rootPath;
}

std::string StaticContentProvider::getResponse(HttpRequest request) {

    if (request.method != "GET") {
        HttpResponse httpResponse = (new HttpResponseBuilder())
                ->buildWithErrorStatus(HTTP_405_METHOD_NOT_ALLOWED);
        return this->httpParser->parseToStringResponse(httpResponse);
    }

    try {
        std::string fullPath = getFullPath(request.uri);
        if (exists(fullPath)) {
            if (is_regular_file(fullPath)) {
                return getFileResponse(request.uri);
            } else if (is_directory(fullPath)) {
                std::string index = getDirectoryPath(fullPath) + "index.html";
                if (exists(index)) {
                    return getDirectoryResponse(index);
                } else {
                    HttpResponse httpResponse = (new HttpResponseBuilder())
                            ->buildWithErrorStatus(HTTP_404_NOT_FOUND);
                    return this->httpParser->parseToStringResponse(httpResponse);
                }
            } else {
                throw ContentProviderRespondingException(request);
            }
        } else {
            HttpResponse httpResponse = (new HttpResponseBuilder())
                    ->buildWithErrorStatus(HTTP_404_NOT_FOUND);
            return this->httpParser->parseToStringResponse(httpResponse);
        }
    } catch (FileReadingException &exception) {
        throw ContentProviderRespondingException(request, exception);
    } catch (FileTypeRecognitionException &exception) {
        throw ContentProviderRespondingException(request, exception);
    }
}

std::string StaticContentProvider::getFileResponse(std::string filename) {
    auto body = getFileContent(getFullPath(filename).c_str());
    std::map<std::string, std::string> headers;
    headers.insert(std::pair<std::string, std::string>("Content-Type", getFileType(filename)));
    HttpResponse httpResponse = (new HttpResponseBuilder())
            ->buildWithOKStatus(HTTP_200_OK, body, headers);
    return this->httpParser->parseToStringResponse(httpResponse);
}

std::string StaticContentProvider::getDirectoryResponse(std::string index) {
    auto body = getFileContent(index.c_str());
    std::map<std::string, std::string> headers;
    headers.insert(std::pair<std::string, std::string>("Content-Type", getFileType("index.html")));
    HttpResponse httpResponse = (new HttpResponseBuilder())
            ->buildWithOKStatus(HTTP_200_OK, body, headers);
    return this->httpParser->parseToStringResponse(httpResponse);
}

std::string StaticContentProvider::getFullPath(std::string uri) {
    return this->rootPath + getFilename(uri);
}

std::string StaticContentProvider::getDirectoryPath(std::string uri) {
    if (uri[uri.length() - 1] != '/') { uri += '/'; }
    return uri;
}


std::string StaticContentProvider::getFilename(std::string uri) {
    if (uri[0] == '/') { uri.erase(0, 1); }
    return uri;
}

std::string StaticContentProvider::getFileContent(const char *filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (file) {
        try {
            std::string contents;
            file.seekg(0, std::ios::end);
            contents.resize(file.tellg());
            file.seekg(0, std::ios::beg);
            file.read(&contents[0], contents.size());
            file.close();
            return (contents);
        }
        catch (std::ifstream::failure &exception) {
            throw FileReadingException();
        }
    } else {
        throw FileReadingException();
    }
}

const char *StaticContentProvider::getFileType(std::string uri) {
    magic_t magicCookie = magic_open(MAGIC_MIME_TYPE);
    magic_load(magicCookie, NULL);
    std::string filename = getFullPath(uri);
    const char *mimetype = magic_file(magicCookie, filename.c_str());
    if (mimetype != NULL) return mimetype;
    else throw FileTypeRecognitionException();
}

StaticContentProvider::~StaticContentProvider() {
    delete (this->httpParser);
}

