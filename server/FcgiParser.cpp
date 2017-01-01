#include <cstring>
#include <iostream>
#include <vector>
#include "FcgiParser.h"

void FcgiParser::parseRequest(std::string request) {
    std::string splitter = "\r\n\r\n";
    std::string header = request.substr(0, request.find(splitter));
    std::string body = request.substr(request.find(splitter)+4); //+4 to avoid splitter in substring

    splitter = "\r\n";
    std::string header_tmp = header;
    std::size_t start = 0;
    std::size_t stop = header_tmp.find(splitter);
    bool last = false;
    while(header_tmp.length()) {
        std::string line = header_tmp.substr(start, stop);

        if(!last) { header_tmp = header_tmp.substr(stop+2); }
        else { header_tmp = ""; }

        stop = header_tmp.find(splitter);
        if(stop == std::string::npos) {
            last = true;
            stop = header_tmp.length();
        }
    }
}

