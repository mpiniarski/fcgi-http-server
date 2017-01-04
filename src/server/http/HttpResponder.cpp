#include <spdlog/spdlog.h>
#include "HttpResponder.h"
#include "../exception/exceptions.h"
#include "../../socket/exception/exceptions.h"

static auto logger = spdlog::stdout_color_mt("HTTP Responder");

void HttpResponder::sendResponse(Socket &socket, HttpResponse &response) {
    std::string httpResponse;
    httpResponse += "HTTP/1.0 "
                    + std::to_string(response.status.code) + " "
                    + response.status.message + "\n";
    try {
        socket.sendMessage(httpResponse);
    }
    catch (SocketResponseSendException &exception) {
        logger->warn("Tried to response with status " + std::to_string(response.status.code) +
                     " but failed because of exception:\n\t" + exception.what());
    }
}

void HttpResponder::sendResponse(Socket &socket, std::string &response) {

}
