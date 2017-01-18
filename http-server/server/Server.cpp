#include "Server.h"
#include "../socket/exceptions.h"
#include "../content/exceptions.h"
#include "http/exceptions.h"
#include <iostream>
#include <spdlog/spdlog.h>
#include <boost/thread/thread.hpp>

static auto logger = spdlog::stdout_color_mt("Server");

Server::Server(HostAddress serverAddress, ContentProvider *dynamicContentProvider,
               ContentProvider *staticContentProvider, std::vector<std::string> dynamicUriPatterns, int timeout) {
    try {
        int socketDescriptor = socket(PF_INET, SOCK_STREAM, 0);
        listenSocket = new Socket(socketDescriptor);
        listenSocket->setReuseAddr();
        listenSocket->bindTo(serverAddress.ip, (uint16_t) serverAddress.port);
        listenSocket->setListen(1);

        this->dynamicContentProvider = dynamicContentProvider;
        this->staticContentProvider = staticContentProvider;
        this->httpParser = new HttpParser();
        this->timeout = timeout;
        for (std::string pattern : dynamicUriPatterns) {
            this->dynamicUriPatterns.push_back(std::regex(pattern));
        }
    }
    catch (SocketException &exception) {
        throw FatalServerException(exception);
    }
    logger->info("Created server at {}:{}", serverAddress.ip, std::to_string(serverAddress.port));
}

void Server::listenForever() {
    while (true) {
        Socket *clientSocket = listenSocket->acceptConnection();
        logger->debug("Accepted new connection");
        std::thread(&Server::handleRequestWithTimeoutSupport, this, std::ref(*clientSocket)).detach();
    }
}

void Server::handleRequestWithTimeoutSupport(Socket &socketConnection) {
    boost::thread handleRequestThread(&Server::handleRequest, this, boost::ref(socketConnection));
    if (!handleRequestThread.try_join_for(boost::chrono::seconds(this->timeout))) {
        handleRequestThread.interrupt();
        logger->warn("Response timeout");
        HttpResponse response = HttpResponse(HTTP_VERSION_1_0, HTTP_504_GATEWAY_TIMEOUT);
        sendResponse(socketConnection, httpParser->parseToStringResponse(response));
    }
    delete (&socketConnection);
}

void Server::handleRequest(Socket &socketConnection) {
    try {
        std::string request = socketConnection.receiveMessage();
        logger->debug("Received request:\n{}", request);
        HttpRequest httpRequest = httpParser->parseToHttpRequest(request);
        std::string httpResponse = isMatchingToDynamicUri(httpRequest) ?
                                   dynamicContentProvider->getResponse(httpRequest) :
                                   staticContentProvider->getResponse(httpRequest);
        boost::this_thread::interruption_point();
        socketConnection.sendMessage(httpResponse);
        logger->debug("Sent response:\n{}", httpResponse);
    }
    catch (ConnectionClosedException &exception) {
        logger->error(exception.what());
    }
    catch (HttpParserException &exception) {
        logger->warn(exception.what());
        boost::this_thread::interruption_point();
        HttpResponse response = HttpResponse(HTTP_VERSION_1_0, HTTP_400_BAD_REQUEST);
        sendResponse(socketConnection, httpParser->parseToStringResponse(response));
    }
    catch (SocketMessageSendException &exception) {
        logger->error(exception.what());
    }
    catch (SocketException &exception) {
        logger->warn(exception.what());
        boost::this_thread::interruption_point();
        HttpResponse response = HttpResponse(HTTP_VERSION_1_0, HTTP_500_INTERNAL_SERVER_ERROR);
        sendResponse(socketConnection, httpParser->parseToStringResponse(response));
    }
    catch (ContentProviderRespondingException &exception) {
        logger->warn(exception.what());
        boost::this_thread::interruption_point();
        HttpResponse response = HttpResponse(HTTP_VERSION_1_0, HTTP_500_INTERNAL_SERVER_ERROR);
        sendResponse(socketConnection, httpParser->parseToStringResponse(response));
    }
    catch (boost::thread_interrupted &exception) {
        logger->warn("Thread interrupted");
    }
}

bool Server::isMatchingToDynamicUri(const HttpRequest &httpRequest) const {
    for (std::regex pattern : dynamicUriPatterns) {
        if (regex_match(httpRequest.uri, pattern)) {
            return true;
        }
    }
    return false;
}

void Server::sendResponse(Socket &socket, std::string response) {
    try {
        socket.sendMessage(response);
    }
    catch (SocketMessageSendException &exception) {
        logger->warn(exception.what());
    }
}

Server::~Server() {
    delete listenSocket;
    delete dynamicContentProvider;
    delete httpParser;
}

