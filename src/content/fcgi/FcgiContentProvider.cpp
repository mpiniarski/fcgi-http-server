#include <spdlog/spdlog.h>
#include "FcgiContentProvider.h"
#include "FcgiCommunicator.h"
#include "fcgi.h"
#include "../../server/exception/exceptions.h"

static auto logger = spdlog::stdout_color_mt("FCGI Content Provider");

HttpResponse FcgiContentProvider::getResponse(HttpRequest request) {
//    FcgiCommunicator communicator = FcgiCommunicator();
//    //TODO Parse HttpRequest to FcgiRequest
//    std::string requestToSend;
//    communicator.sendRequest(requestToSend); // TODO change communicator to receive FcgiRequest
//    FcgiResponse response = communicator.receiveResponse();
//    if (response.protocolStatus == FCGI_REQUEST_COMPLETE) {
//        socketConnection.sendMessage(response.STDOUT); TODO Change to creating proper HttpResponse
//        if (!response.STDERR.empty()) {
//            logger->warn("Fcgi errors:\n\n{}", response.STDERR);
//        }
//    } else if (response.protocolStatus == FCGI_CANT_MPX_CONN || response.protocolStatus == FCGI_OVERLOADED){
//        throw HttpException(HTTP_503_SERVICE_UNAVAILABLE);
//    }
//    else if (response.protocolStatus == FCGI_UNKNOWN_ROLE){
//        throw HttpException(HTTP_500_INTERNAL_SERVER_ERROR);
//    }


    return HttpResponse();
}
