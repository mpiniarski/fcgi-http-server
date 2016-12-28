#include <iostream>
#include "server/Server.h"
#include <spdlog/spdlog.h>

static auto logger = spdlog::stdout_color_mt("HTTP FCGI Server");

int main() {
    spdlog::set_level(spdlog::level::debug);

    try {
        Server server = Server();
        logger->info("Created server at {}:{}", "localhost", "8888");
        server.listenForever();
    }
    catch (FatalServerException &exception) {
        logger->critical(exception.what());
        return -1;
    }
    return 0;
}