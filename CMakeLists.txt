cmake_minimum_required(VERSION 3.6)
project(fcgi_http_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES
        http-server/main.cpp
        http-server/server/Server.cpp
        http-server/server/Server.h
        http-server/server/exception/exceptions.h
        http-server/content/fcgi/communication/FcgiCommunicator.cpp
        http-server/content/fcgi/communication/FcgiCommunicator.h
        http-server/content/fcgi/communication/exceptions.h
        http-server/server/exception/exceptions.h
        http-server/content/fcgi/fcgi.h
        http-server/socket/Socket.cpp
        http-server/socket/Socket.h
        http-server/content/fcgi/FcgiParser.cpp
        http-server/content/fcgi/FcgiParser.h
        http-server/content/ContentProvider.h
        http-server/server/http/http.h
        http-server/content/fcgi/FcgiContentProvider.cpp
        http-server/content/fcgi/FcgiContentProvider.h
        http-server/socket/exceptions.h
        http-server/exceptions.h
        http-server/server/http/HttpParser.cpp
        http-server/server/http/HttpParser.h
        http-server/content/exceptions.h
        )
add_executable(fcgi_http_server ${SOURCE_FILES})