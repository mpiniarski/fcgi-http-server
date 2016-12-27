#include "server/Server.h"

int main() {
    try {
        Server server = Server();
        server.listenForever();
    }
    catch(FatalServerException &exception){
        //TODO log
        return -1;
    }
    return 0;
}