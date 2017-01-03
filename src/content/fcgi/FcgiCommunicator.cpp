#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <map>
#include "FcgiCommunicator.h"
#include "fcgi.h"
#include "FcgiParser.h"
#include "../../server/exception/exceptions.h"
#include "../../socket/exception/exceptions.h"

FcgiCommunicator::FcgiCommunicator() {
    try {
        int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        communicationSocket = new Socket(socketDescriptor);
        communicationSocket->connectWith("127.0.0.1", 8887);
    }
    catch (SocketException &exception) {
        throw FatalServerException(exception);
    }
}

void FcgiCommunicator::sendRequest(const std::string &request) {
    try {
        FcgiParser parser = FcgiParser();
        FcgiRequest fcgiRequest = parser.parseRequest(request);
        sendBeginRecord();
        sendStream(fcgiRequest.body, FCGI_STDIN);
        sendParameters(fcgiRequest.parameters);
    }
    catch (SocketResponseSendException &exception) {
        throw HttpException(HTTP_500_INTERNAL_SERVER_ERROR);
    }
}

void FcgiCommunicator::sendBeginRecord() {
    FCGI_BeginRequestBody body = FCGI_BeginRequestBody(FCGI_RESPONDER, FCGI_KEEP_CONN);
    FCGI_Header header = FCGI_Header(FCGI_BEGIN_REQUEST, 1, sizeof(body), 0);
    communicationSocket->sendMessage(&header, sizeof(header));
    communicationSocket->sendMessage(&body, sizeof(body));
}

void FcgiCommunicator::sendStream(const std::string request, unsigned char type) {
    unsigned long partSize = 65535;
    for (unsigned long i = 0; i < request.length(); i += partSize) {
        std::string contentData = request.substr(i, partSize);
        FCGI_Header header = FCGI_Header(type, 1, (uint16_t) contentData.length(), 0);
        communicationSocket->sendMessage(&header, sizeof(header));
        communicationSocket->sendMessage(contentData);
    }
    FCGI_Header header = FCGI_Header(type, 1, 0, 0);
    communicationSocket->sendMessage(&header, sizeof(header));
}

void FcgiCommunicator::sendParameters(const std::map<std::string, std::string> parameters) {
    std::string contentData = "";
    for (auto param : parameters) {
        contentData += toProperSizeString((uint32_t) param.first.length());
        contentData += toProperSizeString((uint32_t) param.second.length());
        contentData += param.first;
        contentData += param.second;
    }
    sendStream(contentData, FCGI_PARAMS);
}

std::string FcgiCommunicator::toProperSizeString(uint32_t number) {
    std::string result;
    if (number <= 127){
        result += char(number & 0xFF);
    }
    else{
        result += char((number >> 24) & 0xFF);
        result += char((number >> 16) & 0xFF);
        result += char((number >> 8) & 0xFF);
        result += char(number & 0xFF);
    }
    return result;
}

FcgiCommunicator::~FcgiCommunicator() {
    delete (communicationSocket);
}

FcgiResponse FcgiCommunicator::receiveResponse() {
    FcgiResponse fcgiResponse = FcgiResponse();
    while (1) {
        FCGI_Header header = FCGI_Header((void *) communicationSocket->receiveMessage(sizeof(FCGI_Header)).c_str());
        std::string bodyString = communicationSocket->receiveMessage(header.contentLength);
        communicationSocket->receiveMessage(header.paddingLength);
        if (header.type == FCGI_STDOUT) {
            fcgiResponse.STDOUT += bodyString;
        } else if (header.type == FCGI_STDERR) {
            fcgiResponse.STDERR += bodyString;
        } else if (header.type == FCGI_END_REQUEST) {
            FCGI_EndRequestBody endRequestBody = FCGI_EndRequestBody((void *) bodyString.c_str());
            fcgiResponse.appStatus = endRequestBody.appStatus;
            fcgiResponse.protocolStatus = endRequestBody.protocolStatus;
            break;
        }
    }
    return fcgiResponse;
}


