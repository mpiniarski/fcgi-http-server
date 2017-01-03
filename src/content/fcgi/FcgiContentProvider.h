#pragma once

#include "../ContentProvider.h"

class FcgiContentProvider : public ContentProvider{
public:
    virtual HttpResponse getResponse(HttpRequest request) override;

};




