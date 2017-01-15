#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <map>
#include <thread>
#include <zconf.h>
#include "FcgiCommunicator.h"
#include "../fcgi.h"
#include "../FcgiParser.h"
#include "../../../server/exception/exceptions.h"
#include "../../../socket/exceptions.h"
#include "exceptions.h"

FcgiCommunicator::FcgiCommunicator(HostAddress &fcgiAddress) {
    try {
        int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        communicationSocket = new Socket(socketDescriptor);
        communicationSocket->connectWith(fcgiAddress.ip, (uint16_t) fcgiAddress.port);

        isListening = true;
        listener = std::thread(&FcgiCommunicator::listenForResponses, this);
    }
    catch (SocketException &exception) {
        throw FcgiCommunicationEstablishException(exception);
    }

}

void FcgiCommunicator::sendRequest(FcgiRequest &request) {
    try {
        sendBeginRecord(request.id);
        sendStream(request.id, request.body, FCGI_STDIN);
        sendParameters(request.id, request.parameters);
    }
    catch (SocketException &exception) {
        throw FcgiCommunicationRequestSendException(exception);
    }
}

void FcgiCommunicator::sendBeginRecord(uint16_t requestId) {
    FCGI_Record_BeginRequestBody body = FCGI_Record_BeginRequestBody(FCGI_RESPONDER, FCGI_KEEP_CONN);
    FCGI_Record_Header header = FCGI_Record_Header(FCGI_BEGIN_REQUEST, requestId, sizeof(body), 0);
    communicationSocket->sendMessage(&header, sizeof(header));
    communicationSocket->sendMessage(&body, sizeof(body));
}

void FcgiCommunicator::sendStream(uint16_t requestId, const std::string content, unsigned char type) {
    unsigned long partSize = 65535;
    for (unsigned long i = 0; i < content.length(); i += partSize) {
        std::string contentData = content.substr(i, partSize);
        FCGI_Record_Header header = FCGI_Record_Header(type, requestId, (uint16_t) contentData.length(), 0);
        communicationSocket->sendMessage(&header, sizeof(header));
        communicationSocket->sendMessage(contentData);
    }
    FCGI_Record_Header header = FCGI_Record_Header(type, requestId, 0, 0);
    communicationSocket->sendMessage(&header, sizeof(header));
}

void FcgiCommunicator::sendParameters(uint16_t requestId, const std::map<std::string, std::string> parameters) {
    std::string contentData = "";
    for (auto param : parameters) {
        contentData += toProperSizeString((uint32_t) param.first.length());
        contentData += toProperSizeString((uint32_t) param.second.length());
        contentData += param.first;
        contentData += param.second;
    }
    sendStream(requestId, contentData, FCGI_PARAMS);
}

std::string FcgiCommunicator::toProperSizeString(uint32_t number) {
    std::string result;
    if (number <= 127) {
        result += char(number & 0xFF);
    } else {
        result += char((number >> 24) & 0xFF);
        result += char((number >> 16) & 0xFF);
        result += char((number >> 8) & 0xFF);
        result += char(number & 0xFF);
    }
    return result;
}


void FcgiCommunicator::listenForResponses() {
    while (isListening) {
        try {
            FCGI_Record_Header header = FCGI_Record_Header(
                    (void *) communicationSocket->receiveMessage(sizeof(FCGI_Record_Header)).c_str());
            std::string bodyString = communicationSocket->receiveMessage(header.contentLength);
            communicationSocket->receiveMessage(header.paddingLength);

            FcgiResponse &response = responseMap[header.requestId];

            if (header.type == FCGI_STDOUT) {
                response.STDOUT += bodyString;
            } else if (header.type == FCGI_STDERR) {
                response.STDERR += bodyString;
            } else if (header.type == FCGI_END_REQUEST) {
                FCGI_Record_EndRequestBody endRequestBody = FCGI_Record_EndRequestBody((void *) bodyString.c_str());
                response.appStatus = endRequestBody.appStatus;
                response.protocolStatus = endRequestBody.protocolStatus;
                response.isFinished = true;
            }
        }
        catch (ConnectionClosedException &exception) {
            throw FcgiCommunicationResponseReceiveException(exception);
        }
        catch (SocketException &exception) {
            throw FcgiCommunicationResponseReceiveException(exception);
        }
    }
}

FcgiResponse FcgiCommunicator::receiveResponse(int requestId) {
    while (true) {
        FcgiResponse &response = responseMap[requestId];
        if (response.isFinished) {
            return response;
        } else {
            usleep(100);
        }
    }
}

FcgiCommunicator::~FcgiCommunicator() {
    isListening = false;
    listener.join();
    delete (communicationSocket);
}



