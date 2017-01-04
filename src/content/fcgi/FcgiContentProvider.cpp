#include <spdlog/spdlog.h>
#include "FcgiContentProvider.h"
#include "fcgi.h"
#include "../../server/exception/exceptions.h"
#include "FcgiParser.h"
#include "communication/exceptions.h"
#include "../exceptions.h"

static auto logger = spdlog::stdout_color_mt("FCGI Content Provider");

std::string FcgiContentProvider::getResponse(HttpRequest request) {
    try {
        //TODO Parse HttpRequest to FcgiRequest
        std::string requestToSend;
        FcgiParser parser = FcgiParser();
        FcgiRequest fcgiRequest = parser.parseRequest(requestToSend);
        fcgiCommunicator->sendRequest(fcgiRequest);
        FcgiResponse response = fcgiCommunicator->receiveResponse();
        if (response.protocolStatus == FCGI_REQUEST_COMPLETE) {
//        socketConnection.sendMessage(response.STDOUT); TODO Change to creating proper HttpResponse
            if (!response.STDERR.empty()) {
                logger->warn("Fcgi errors:\n\n{}", response.STDERR);
            }
        } else if (response.protocolStatus == FCGI_CANT_MPX_CONN || response.protocolStatus == FCGI_OVERLOADED) {
            // HttpResponse(HTTP_VERSION_1_0, HTTP_503_SERVICE_UNAVAILABLE); // TODO Parse to strign
            return std::string();
        } else if (response.protocolStatus == FCGI_UNKNOWN_ROLE) {
            // HttpResponse(HTTP_VERSION_1_0, HTTP_500_SERVICE_UNAVAILABLE); // TODO Parse to strign
            return std::string();
        }
        return std::string();
    }
    catch(FcgiCommunicationException& exception){
        throw UnableToProvideContentException(request);
    }
}

FcgiContentProvider::FcgiContentProvider() {
    FcgiCommunicator communicator = FcgiCommunicator();
}

FcgiContentProvider::~FcgiContentProvider() {
    delete(fcgiCommunicator);
}
