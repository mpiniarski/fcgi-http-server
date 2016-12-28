cmake_minimum_required(VERSION 3.6)
project(fcgi_http_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        main.cpp
        server/Server.cpp
        server/Server.h
        server/exception/exceptions.h
        server/FcgiCommunicator.cpp
        server/FcgiCommunicator.h
        server/exception/exceptions.h
        server/CommunicationSocket.cpp
        server/CommunicationSocket.h
        fcgi.h
        )
add_executable(fcgi_http_server ${SOURCE_FILES})