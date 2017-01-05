#include <spdlog/spdlog.h>
#include "HttpResponder.h"
#include "../exception/exceptions.h"
#include "../../socket/exception/exceptions.h"

static auto logger = spdlog::stdout_color_mt("HTTP Responder");

void HttpResponder::sendResponse(Socket &socket, HttpResponse &response) {
    std::string lineSeparator = "\r\n";
    std::string httpResponse;
    httpResponse += response.version + " "
                    + std::to_string(response.status.code) + " "
                    + response.status.message + lineSeparator;
    for(auto header : response.headers){
        httpResponse += header.first + ": " + header.second + lineSeparator;
    }
    httpResponse += lineSeparator + response.body;
    sendResponse(socket, httpResponse);
}

void HttpResponder::sendResponse(Socket &socket, std::string &response) {
    try {
        socket.sendMessage(response);
    }
    catch (SocketResponseSendException &exception) {
        logger->warn(exception.what());
    }
}
