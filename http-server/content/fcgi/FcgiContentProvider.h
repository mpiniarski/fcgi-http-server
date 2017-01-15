#pragma once

#include "../ContentProvider.h"
#include "communication/FcgiCommunicator.h"
#include "FcgiParser.h"
#include "../../server/http/HttpParser.h"
#include "../../config/ConfigProvider.h"

class FcgiContentProvider : public ContentProvider{
public:
    FcgiContentProvider(HostAddress fcgiAddress, HostAddress serverAddress);

    virtual std::string getResponse(HttpRequest request) override;

    virtual ~FcgiContentProvider();

private:
    std::string generateStringHttpResponse(FcgiResponse& response);
    FcgiCommunicator * fcgiCommunicator;
    FcgiParser* fcgiParser;
    HttpParser* httpParser;

    HostAddress serverAddress;

    std::string fixResponse(std::string result) const;

    void addServerParameters(FcgiRequest &fcgiRequest) const;
};




