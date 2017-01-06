#pragma once

#include "../exceptions.h"

class ConfigException : public Exception {
public:
    ConfigException(const std::string &message) : Exception(message) {}

public:
    ConfigException(const std::string &message, Exception &parent) : Exception(message, parent) {}
};

class ConfigFileOpenException : public ConfigException{
public:
    ConfigFileOpenException(const std::string &filePath) :
            ConfigException("Unable to open config file from path: " + filePath) {}
};

class ConfigMissingRequiresOptionException : public ConfigException {
public:
    ConfigMissingRequiresOptionException(const std::string &optionName) :
            ConfigException("Missing requires parameter: " + optionName) {}
};
