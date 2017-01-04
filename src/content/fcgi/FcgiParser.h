#pragma once
#include <string>
#include <map>
#include <vector>
#include "communication/FcgiCommunicator.h"
#include "../../server/http/http.h"


class FcgiParser {
public:
    FcgiRequest parseToFcgiRequest(HttpRequest httpRequest);

private:
    void convertHeadersToParameters(std::map<std::string,std::string> headers, FcgiRequest &fcgiRequest);

    void convertPathToParameters(std::string path, FcgiRequest &fcgiRequest);
};


