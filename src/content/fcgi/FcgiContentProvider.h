#pragma once

#include "../ContentProvider.h"
#include "communication/FcgiCommunicator.h"

class FcgiContentProvider : public ContentProvider{
public:
    FcgiContentProvider();

    virtual std::string getResponse(HttpRequest request) override;

    virtual ~FcgiContentProvider();

private:
    FcgiCommunicator * fcgiCommunicator;

};




