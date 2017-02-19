#include <spdlog/spdlog.h>
#include "FcgiContentProvider.h"
#include "fcgi.h"
#include "communication/exceptions.h"
#include "../exceptions.h"

static auto logger = spdlog::stdout_color_mt("FCGI Content Provider");

FcgiContentProvider::FcgiContentProvider(HostAddress fcgiAddress, HostAddress serverAddress) {
    try {
        fcgiCommunicator = new FcgiCommunicator(fcgiAddress);
    }
    catch (FcgiCommunicationEstablishException &exception) {
        throw ContentProviderCreatingException(exception);
    }
    fcgiParser = new FcgiParser();
    httpParser = new HttpParser();
    this->serverAddress = serverAddress;
    logger->debug("Created Fcgi content provider");
}

std::string FcgiContentProvider::getResponse(HttpRequest request) {
    try {
        FcgiRequest fcgiRequest = fcgiParser->parseToFcgiRequest(request);
        addServerParameters(fcgiRequest);
        fcgiCommunicator->sendRequest(fcgiRequest);
        logger->debug("Sent request with id : " + std::to_string(fcgiRequest.id));
        FcgiResponse response = fcgiCommunicator->receiveResponse(fcgiRequest.id);
        logger->debug("Received response with id : " + std::to_string(fcgiRequest.id));
        if (!response.STDERR.empty()) {
            logger->warn("Fcgi errors for request {} {}:\n{}", request.method, request.uri, response.STDERR);
        }
        return generateStringHttpResponse(response);
    }
    catch (FcgiCommunicationException &exception) {
        throw ContentProviderRespondingException(request, exception);
    }
}

void FcgiContentProvider::addServerParameters(FcgiRequest &fcgiRequest) const {
    fcgiRequest.parameters.insert(std::pair<std::string, std::string>("SERVER_NAME", serverAddress.ip));
    fcgiRequest.parameters.insert(
            std::pair<std::string, std::string>("SERVER_PORT", std::to_string(serverAddress.port)));
}

std::string FcgiContentProvider::generateStringHttpResponse(FcgiResponse &response) {
    std::string result;
    if (response.protocolStatus == FCGI_REQUEST_COMPLETE) {
        result = response.STDOUT;
        // TODO Decide what to do with that. Is it specific for our FCGI application or is it standard? :
        result = fixResponse(result);
    } else if (response.protocolStatus == FCGI_CANT_MPX_CONN || response.protocolStatus == FCGI_OVERLOADED) {
        result = httpParser->parseToStringResponse((new HttpResponseBuilder())
                                                           ->buildWithErrorStatus(HTTP_503_SERVICE_UNAVAILABLE)
        );
    } else if (response.protocolStatus == FCGI_UNKNOWN_ROLE) {
        result = httpParser->parseToStringResponse((new HttpResponseBuilder())
                                                           ->buildWithErrorStatus(HTTP_500_INTERNAL_SERVER_ERROR)
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
