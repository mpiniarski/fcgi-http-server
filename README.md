# HTTP server with FCGI support#
This is an implementation of an HTTP server with FastCGI support.

## Quick start ##
1. Download this repository

    ```
    git clone https://github.com/mpiniarski/fcgi-http-server.git 
    ```
2. Change directory

    ```
    cd fcgi-http-server/http-server
    ```
3. Build 

    ```
    cmake CMakeLists.txt
    make
    ```
4. Run server (e.g. with help option)

    ```
    ./fcgi_http_server --help
    ```
    You have to provide at least _--fcgi-port_ option for server to start. It will run on default options.
    
## FCGI app ##
This repository contains a FCGI applicaton implemented in Python web microframework - Flask. It can be used to test the server. It runs on localhost:8887.

