#pragma once

#include "../ContentProvider.h"

class StaticContentProvider : public ContentProvider {
public:
    virtual std::string getResponse(HttpRequest request) override;

private:
    std::string getFullPath(std::string uri);
    std::string getFilename(std::string uri);
    std::string getFileContent(const char *filename);
    void getFileResponse(std::string path, HttpResponse &httpResponse);
    void getFileType(std::string uri, HttpResponse &httpResponse);
};


