#include <iostream>
#include "server/Server.h"

int main() {
    Server server = Server();
    server.listenForever();

    return 0;
}