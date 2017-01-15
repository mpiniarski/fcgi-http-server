#include "ConfigProvider.h"
#include "exceptions.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>

namespace po = boost::program_options;

ConfigProvider::ConfigProvider(int ac, char **av) {
    const char *LOCALHOST_VALUE = "127.0.0.1";
    const char *SERVER_IP_OPTION = "server_ip";
    const char *SERVER_PORT_OPTION = "server_port";
    const char *FCGI_IP_OPTION = "fcgi_ip";
    const char *FCGI_PORT_OPTION = "fcgi_port";
    const char *DEBUG_OPTION = "debug";
    const char *HELP_OPTION = "help";
    const char *CONFIG_OPTION = "config";

    try {
        std::string configFilePath;
        bool isHelpOption;

        po::options_description generic("Generic options");
        generic.add_options()
                ((std::string(HELP_OPTION) + ",h").c_str(), po::bool_switch(&isHelpOption)->default_value(false),
                 "produce help message")
                ((std::string(DEBUG_OPTION) + ",d").c_str(), po::bool_switch(&this->debug)->default_value(false),
                 "turn on debug messages")
                ((std::string(CONFIG_OPTION) + ",c").c_str(), po::value<std::string>(&configFilePath),
                 "path to config file");

        po::options_description config("Configuration");
        config.add_options()
                (SERVER_IP_OPTION, po::value<std::string>(&this->serverAddress.ip)->default_value(LOCALHOST_VALUE),
                 "set server ip address")
                (SERVER_PORT_OPTION, po::value<int>(&this->serverAddress.port)->default_value(8000), "set server port")
                (FCGI_IP_OPTION, po::value<std::string>(&this->fcgiAppAddres.ip)->default_value(LOCALHOST_VALUE),
                 "set fcgi application ip address")
                (FCGI_PORT_OPTION, po::value<int>(&this->fcgiAppAddres.port)->required(), "set fcgi application port");

        po::options_description all("Allowed options");
        all.add(generic).add(config);

        po::variables_map vm;
        po::store(po::command_line_parser(ac, (const char *const *) av)
                          .options(generic)
                          .allow_unregistered()
                          .run(), vm);
        po::notify(vm);

        if (isHelpOption) {
            std::cout << all << std::endl;
            exit(1);
        }

        po::store(po::parse_command_line(ac, (const char *const *) av, all), vm);
        if (configFilePath != "") {
            std::ifstream configFile(configFilePath.c_str());
            if (configFile.fail()) {
                throw ConfigFileOpenException(configFilePath);
            }
            po::store(po::parse_config_file(configFile, config), vm);
        }
        po::notify(vm);

    }
    catch (po::error &exception) {
        throw ConfigException(exception.what());
    }
}

const HostAddress ConfigProvider::getServerAddress() {
    return serverAddress;
}

const HostAddress ConfigProvider::getFcgiAppAddres() {
    return fcgiAppAddres;
}

bool ConfigProvider::isDebug() const {
    return debug;
}
