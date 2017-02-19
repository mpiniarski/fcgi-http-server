#pragma once

#include <string>
#include <map>

struct HttpStatus {
    HttpStatus(int code, const std::string &message) : code(code), message(message) {}

    int code;
    std::string message;
};

const std::string HTTP_VERSION_1_0 = "HTTP/1.0";

const HttpStatus HTTP_200_OK = HttpStatus(200, "OK");
const HttpStatus HTTP_400_BAD_REQUEST = HttpStatus(400, "BAD REQUEST");
const HttpStatus HTTP_404_NOT_FOUND = HttpStatus(404, "NOT FOUND");
const HttpStatus HTTP_405_METHOD_NOT_ALLOWED = HttpStatus(405, "METHOD NOT ALLOWED");
const HttpStatus HTTP_500_INTERNAL_SERVER_ERROR = HttpStatus(500, "INTERNAL SERVER ERROR");
const HttpStatus HTTP_503_SERVICE_UNAVAILABLE = HttpStatus(503, "SERVICE UNAVAILABLE");
const HttpStatus HTTP_504_GATEWAY_TIMEOUT = HttpStatus(504, "GATEWAY TIMEOUT");

struct HttpRequest {
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

struct HttpResponse {
    std::string version;
    HttpStatus status;
    std::map<std::string, std::string> headers;
    std::string body;

    HttpResponse(const std::string &version,
                 const HttpStatus &status,
                 const std::string &body,
                 const std::map<std::string, std::string> &headers
    ) : version(version),
        status(status),
        headers(headers),
        body(body) {
        if(body.length() > 0) {
            this->headers.insert(std::pair<std::string, std::string>("Content-Length", std::to_string(body.length())));
            if(this->headers.find("Content-Type") == this->headers.end()) {
                this->headers.insert(std::pair<std::string, std::string>("Content-Type", "text/plain"));
            }
        }
    }
};

class HttpResponseBuilder {
private:
    std::string version;
    const HttpStatus *status;
    std::string body = "";
    std::map<std::string, std::string> headers = std::map<std::string, std::string>();

public:
    HttpResponse build() {
        return HttpResponse(version, *status, body, headers);
    }

    HttpResponse buildWithErrorStatus(const HttpStatus &status) {
        HttpResponse httpResponse = (new HttpResponseBuilder())
                ->withVersion(HTTP_VERSION_1_0)
                ->withStatus(status)
                ->withBody(status.message)
                ->build();
        return httpResponse;
    }

    HttpResponse buildWithOKStatus(const HttpStatus &status, std::string body, std::map<std::string, std::string> headers) {
        HttpResponse httpResponse = (new HttpResponseBuilder())
                ->withVersion(HTTP_VERSION_1_0)
                ->withStatus(status)
                ->withBody(body)
                ->withHeaders(headers)
                ->build();
        return httpResponse;
    }

    HttpResponseBuilder *withVersion(std::string version) {
        this->version = version;
        return this;
    }

    HttpResponseBuilder *withStatus(const HttpStatus &status) {
        this->status = &status;
        return this;
    }

    HttpResponseBuilder *withBody(std::string body) {
        this->body = body;
        return this;
    }

    HttpResponseBuilder *withHeaders(std::map<std::string, std::string> headers) {
        this->headers = headers;
        return this;
    }
};
