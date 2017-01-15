#pragma once

#include "../ContentProvider.h"

class StaticContentProvider : public ContentProvider {
public:
    virtual std::string getResponse(HttpRequest request) override;

private:
    bool checkIfFileExists(std::string path);
};


