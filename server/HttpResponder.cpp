#include <spdlog/spdlog.h>
#include "HttpResponder.h"
#include "exception/exceptions.h"

static auto logger = spdlog::stdout_color_mt("HTTP Responder");

void HttpResponder::sendResponse(Socket &socket, HttpResponse &response) {
    std::string httpResponse;
    httpResponse += "HTTP/1.0 "
                    + std::to_string(response.getStatus().status) + " "
                    + response.getStatus().message + "\n";
    try {
        socket.sendMessage(httpResponse);
    }
    catch (SocketResponseSendException &exception) {
        logger->warn("Tried to response with status " + std::to_string(response.getStatus().status) +
                     " but failed because of exception:\n\t" + exception.what());
    }
}
