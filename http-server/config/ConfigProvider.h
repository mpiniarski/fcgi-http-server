#pragma once

#include <stdint-gcc.h>
#include <string>

struct HostAddress{
    std::string ip;
    int port;
};

class ConfigProvider {
public:
    ConfigProvider(int ac, char **av);

    const HostAddress getServerAddress();
    const HostAddress getFcgiAppAddres();

    int getTimeout() const;

    bool isDebug() const;

private:
    HostAddress serverAddress;
    HostAddress fcgiAppAddres;
    bool debug;
    int timeout;
};




