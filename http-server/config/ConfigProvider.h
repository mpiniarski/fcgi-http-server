#pragma once

#include <string>
#include <vector>

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
    std::vector<std::string> getDynamicUriPatterns() const;
    const std::string getRootPath() const;

private:
    HostAddress serverAddress;
    HostAddress fcgiAppAddress;
    bool debug;
    int timeout;
    std::vector<std::string> dynamicUriPatterns;
    std::string rootPath;
};

