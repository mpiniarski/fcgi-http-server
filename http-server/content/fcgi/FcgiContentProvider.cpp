#include <spdlog/spdlog.h>
#include "FcgiContentProvider.h"
#include "fcgi.h"
#include "communication/exceptions.h"
#include "../exceptions.h"
#include "../../server/exception/exceptions.h"

static auto logger = spdlog::stdout_color_mt("FCGI Content Provider");

FcgiContentProvider::FcgiContentProvider(HostAddress fcgiAddress) {
    try{
        fcgiCommunicator = new FcgiCommunicator(fcgiAddress);
    }
    catch(FcgiCommunicationEstablishException& exception){
        throw ContentProviderCreatingException(exception);
    }
    fcgiParser = new FcgiParser();
    httpParser = new HttpParser();

}

std::string FcgiContentProvider::getResponse(HttpRequest request) {
    try {
        FcgiRequest fcgiRequest = fcgiParser->parseToFcgiRequest(request);
        fcgiCommunicator->sendRequest(fcgiRequest);
        FcgiResponse response = fcgiCommunicator->receiveResponse();
        if (!response.STDERR.empty()) {
            logger->warn("Fcgi errors for request {} {}:\n{}", request.method, request.uri, response.STDERR);
        }
        return generateStringHttpResponse(response);
    }
    catch (FcgiCommunicationException &exception) {
        throw ContentProviderRespondingException(request, exception);
    }
}

std::string FcgiContentProvider::generateStringHttpResponse(FcgiResponse &response) {
    std::string result;
    if (response.protocolStatus == FCGI_REQUEST_COMPLETE) {
        result = response.STDOUT;
        // TODO Decide what to do with that. Is it specific for our FCGI application or is it standard? :
        result = fixResponse(result);
    } else if (response.protocolStatus == FCGI_CANT_MPX_CONN || response.protocolStatus == FCGI_OVERLOADED) {
        result = httpParser->parseToStringResponse(
                HttpResponse(HTTP_VERSION_1_0, HTTP_503_SERVICE_UNAVAILABLE)
        );
    } else if (response.protocolStatus == FCGI_UNKNOWN_ROLE) {
        result = httpParser->parseToStringResponse(
                HttpResponse(HTTP_VERSION_1_0, HTTP_500_INTERNAL_SERVER_ERROR)
        );
    }
    return result;
}

std::string FcgiContentProvider::fixResponse(std::string result) const {
    std::string replaced = "Status:";
    result.replace(
            result.find(replaced),
            replaced.length(),
            HTTP_VERSION_1_0
    );
    return result;
}

FcgiContentProvider::~FcgiContentProvider() {
    delete (fcgiCommunicator);
    delete (fcgiParser);
    delete (httpParser);
}
