#pragma once

#include <string>
#include "../../socket/Socket.h"
#include "http.h"


class HttpResponder {
public:
    void sendResponse(Socket &socket, HttpResponse& response);

    void sendResponse(Socket &socket, std::string& response);
};




