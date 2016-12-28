#include <iostream>
#include "server/Server.h"

int main() {
    try {
        Server server = Server();
        server.listenForever();
    }
    catch(FatalServerException &exception){
        //TODO log
        std::cout<<exception.what();
        return -1;
    }
    return 0;
}