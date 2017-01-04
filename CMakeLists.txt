cmake_minimum_required(VERSION 3.6)
project(fcgi_http_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

set(SOURCE_FILES
        src/main.cpp
        src/server/Server.cpp
        src/server/Server.h
        src/server/exception/exceptions.h
        src/content/fcgi/communication/FcgiCommunicator.cpp
        src/content/fcgi/communication/FcgiCommunicator.h
        src/content/fcgi/communication/exceptions.h
        src/server/exception/exceptions.h
        src/content/fcgi/fcgi.h
        src/socket/Socket.cpp
        src/socket/Socket.h
        src/content/fcgi/FcgiParser.cpp
        src/content/fcgi/FcgiParser.h
        src/server/http/HttpResponder.cpp src/server/http/HttpResponder.h src/content/ContentProvider.h src/server/http/http.h src/content/fcgi/FcgiContentProvider.cpp src/content/fcgi/FcgiContentProvider.h src/socket/exception/exceptions.h src/exceptions.h src/content/exceptions.h)
add_executable(fcgi_http_server ${SOURCE_FILES})