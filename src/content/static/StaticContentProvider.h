#pragma once

#include "../ContentProvider.h"

class StaticContentProvider : public ContentProvider{
public:
    virtual HttpResponse getResponse(HttpRequest request) override;

};




