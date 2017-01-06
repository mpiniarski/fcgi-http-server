cmake_minimum_required(VERSION 3.6)
project(fcgi_http_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES
        main.cpp
        server/Server.cpp
        server/Server.h
        server/exception/exceptions.h
        content/fcgi/communication/FcgiCommunicator.cpp
        content/fcgi/communication/FcgiCommunicator.h
        content/fcgi/communication/exceptions.h
        server/exception/exceptions.h
        content/fcgi/fcgi.h
        socket/Socket.cpp
        socket/Socket.h
        content/fcgi/FcgiParser.cpp
        content/fcgi/FcgiParser.h
        content/ContentProvider.h
        server/http/http.h
        content/fcgi/FcgiContentProvider.cpp
        content/fcgi/FcgiContentProvider.h
        socket/exceptions.h
        exceptions.h
        server/http/HttpParser.cpp
        server/http/HttpParser.h
        content/exceptions.h
        )
add_executable(fcgi_http_server ${SOURCE_FILES})