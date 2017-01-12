#include <iostream>
#include "server/Server.h"
#include "server/exception/exceptions.h"
#include "content/fcgi/FcgiContentProvider.h"
#include <spdlog/spdlog.h>

std::string logo = "\n"
        "       _    _ _______ _______ _____       _____\n"
        "      | |  | |__   __|__   __|  __ \\     / ____|\n"
        "      | |__| |  | |     | |  | |__) |   | (___   ___ _ ____   _____ _ __\n"
        "      |  __  |  | |     | |  |  ___/     \\___ \\ / _ \\ '__\\ \\ / / _ \\ '__|\n"
        "      | |  | |  | |     | |  | |         ____) |  __/ |   \\ V /  __/ |\n"
        "      |_|  |_|  |_|     |_|  |_|        |_____/ \\___|_|    \\_/ \\___|_|\n";

static auto logger = spdlog::stdout_color_mt("HTTP FCGI Server");

int main(int ac, char **av) {

    logger->info(logo);

    try {
        ConfigProvider serverConfigProvider = ConfigProvider(ac, av);
        if (serverConfigProvider.isDebug()) {
            spdlog::set_level(spdlog::level::debug);
        }
        ContentProvider *contentProvider = new FcgiContentProvider(serverConfigProvider.getFcgiAppAddres());
        Server server = Server(serverConfigProvider.getServerAddress(), contentProvider);
        server.listenForever();
    }
    catch (Exception &exception) {
        logger->critical(exception.what());
        return -1;
    }
    return 0;
}