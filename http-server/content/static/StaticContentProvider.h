#pragma once

#include "../ContentProvider.h"
#include "../../server/http/HttpParser.h"

class StaticContentProvider : public ContentProvider {
public:
    virtual std::string getResponse(HttpRequest request) override;

    StaticContentProvider(std::string rootPath);

    virtual ~StaticContentProvider();

private:
    HttpParser* httpParser;
    std::string rootPath;

    std::string getFullPath(std::string uri);
    std::string getDirectoryPath(std::string uri);
    std::string getFilename(std::string uri);
    std::string getFileContent(const char *filename);
    const char* getFileType(std::string uri);

    std::string getFileResponse(std::string filename);
    std::string getDirectoryResponse(std::string index);
};


