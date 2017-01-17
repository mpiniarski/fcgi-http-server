#pragma once

#include "../ContentProvider.h"

class StaticContentProvider : public ContentProvider {
public:
    virtual std::string getResponse(HttpRequest request) override;

private:
    std::string getFullPath(std::string uri);
    std::string getFilename(std::string uri);
    std::string getFileContent(const char *filename);
    void getFileType(std::string uri, HttpResponse &httpResponse);

    void getFileResponse(std::string filename, HttpResponse &httpResponse);
    void getDirectoryResponse(std::string index, HttpResponse &httpResponse);
};


