#include "StaticContentProvider.h"
#include <boost/filesystem.hpp>
#include <iostream>

using namespace boost::filesystem;

std::string StaticContentProvider::getResponse(HttpRequest request) {
    std::string response;

    if (checkIfFileExists(request.uri)) {
        response = "File exists";
    }
    else {
        response = "File does not exist";
    }

    return response;
}

bool StaticContentProvider::checkIfFileExists(std::string path) {
    if( path[0] == '/' ) { path.erase(0, 1); }
    path = "/home/joanna/http-files/" + path;
    if (exists(path))
    {
        if (is_regular_file(path)) {
            std::cout << path << " size is " << file_size(path) << '\n';
            return true;
        }

        else if (is_directory(path)) {
            std::cout << path << " is a directory\n";
            return false;
        }

        else {
            std::cout << path << " exists, but is not a regular file or directory\n";
            return false;
        }
    }
    else {
        std::cout << path << " does not exist\n";
        return false;
    }
}

