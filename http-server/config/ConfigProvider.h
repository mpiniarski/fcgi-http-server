#pragma once

#include <stdint-gcc.h>
#include <string>

struct HostAddress{
    std::string ip;
    uint16_t port;
};

class ServerConfigProvider {
public:
    ServerConfigProvider(int ac, char **av);

    const HostAddress getServerAddress();
    const HostAddress getFcgiAppAddres();

    bool isDebug() const;

private:
    HostAddress serverAddress;
    HostAddress fcgiAppAddres;
    bool debug;
};




