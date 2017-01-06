# Http server implementing FCGI #
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
This repository contains a FCGI applicaton implemented in Python web microframework: Flask. It can be used to test server. It runs on localhost:8887.

## Future features ##
1. Support for serving static content.
2. Configuration of routing for static and dynamic (fcgi) content.
3. Support for timeout of server response.
4. Support for server concurrency.






