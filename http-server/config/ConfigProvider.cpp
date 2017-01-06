#include "ConfigProvider.h"
#include "exceptions.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

namespace po = boost::program_options;

ServerConfigProvider::ServerConfigProvider(int ac, char **av) {
    const char *LOCALHOST_VALUE = "127.0.0.1";
    const char *SERVER_IP_OPTION = "server_ip";
    const char *SERVER_PORT_OPTION = "server_port";
    const char *FCGI_IP_OPTION = "fcgi_ip";
    const char *FCGI_PORT_OPTION = "fcgi_port";
    const char *DEBUG_OPTION = "debug";
    const char *HELP_OPTION = "help";
    const char *CONFIG_OPTION = "config";

    try {
        po::options_description generic("Generic options");
        generic.add_options()
                ((std::string(HELP_OPTION)+",h").c_str(), "produce help message")
                ((std::string(DEBUG_OPTION)+",d").c_str(), "turn on debug messages")
                ((std::string(CONFIG_OPTION)+",c").c_str(), po::value<std::string>(), "path to config file");
        po::options_description config("Configuration");
        config.add_options()
                (SERVER_IP_OPTION, po::value<std::string>()->default_value(LOCALHOST_VALUE), "set server ip address")
                (SERVER_PORT_OPTION, po::value<int>()->default_value(8000), "set server port")
                (FCGI_IP_OPTION, po::value<std::string>()->default_value(LOCALHOST_VALUE),
                 "set fcgi application ip address")
                (FCGI_PORT_OPTION, po::value<int>()->required(), "set fcgi application port");

        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config);

        po::options_description config_file_options;
        config_file_options.add(config);

        po::options_description all("Allowed options");
        all.add(generic).add(config);


        po::variables_map vm;
        po::store(po::parse_command_line(ac, (const char *const *) av, cmdline_options), vm);

        if (vm.count(HELP_OPTION)) {
            std::cout << all << std::endl;
            exit(1);
        }

        if (vm.count(CONFIG_OPTION)) {
            std::string filePath = vm[CONFIG_OPTION].as<std::string>();
            std::ifstream configFile(filePath.c_str());
            if (configFile.fail()) {
                throw ConfigFileOpenException(filePath);
            }
            po::store(po::parse_config_file(configFile, cmdline_options), vm);
        }
        po::notify(vm);

        this->serverAddress.ip = vm[SERVER_IP_OPTION].as<std::string>();
        this->serverAddress.port = (uint16_t) vm[SERVER_PORT_OPTION].as<int>();
        this->fcgiAppAddres.ip = vm[FCGI_IP_OPTION].as<std::string>();
        this->fcgiAppAddres.port = (uint16_t) vm[FCGI_PORT_OPTION].as<int>();
        this->debug = (bool) vm.count(DEBUG_OPTION);
    }
    catch (po::error& exception){
        throw ConfigException(exception.what());
    }
}

const HostAddress ServerConfigProvider::getServerAddress() {
    return serverAddress;
}

const HostAddress ServerConfigProvider::getFcgiAppAddres() {
    return fcgiAppAddres;
}

bool ServerConfigProvider::isDebug() const {
    return debug;
}
