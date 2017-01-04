#pragma once

#include "../ContentProvider.h"
#include "communication/FcgiCommunicator.h"
#include "FcgiParser.h"
#include "../../server/http/HttpParser.h"

class FcgiContentProvider : public ContentProvider{
public:
    FcgiContentProvider();

    virtual std::string getResponse(HttpRequest request) override;

    virtual ~FcgiContentProvider();

private:
    std::string generateStringHttpResponse(FcgiResponse& response);
    FcgiCommunicator * fcgiCommunicator;
    FcgiParser* fcgiParser;
    HttpParser* httpParser;

    std::string fixResponse(std::string result) const;
};




