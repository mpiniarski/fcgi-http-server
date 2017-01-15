#pragma once

#include <string>
#include <map>

struct HttpStatus {
    HttpStatus(int code, const std::string &message) : code(code), message(message) {}

    HttpStatus() {} //TODO only to get things working - delete later

    int code;
    std::string message;
};

const std::string HTTP_VERSION_1_0 = "HTTP/1.0";
const std::string HTTP_VERSION_1_1 = "HTTP/1.1";

const HttpStatus HTTP_100_CONTINUE = HttpStatus(100, "Continue");
const HttpStatus HTTP_101_SWITCHING_PROTOCOLS = HttpStatus(101, "Switching Protocols");
const HttpStatus HTTP_200_OK = HttpStatus(200, "OK");
const HttpStatus HTTP_201_CREATED = HttpStatus(201, "CREATED");
const HttpStatus HTTP_202_ACCEPTED = HttpStatus(202, "ACCEPTED");
const HttpStatus HTTP_203_NON_AUTHORITATIVE_INFORMATION = HttpStatus(203, "NON AUTHORITATIVE INFORMATION");
const HttpStatus HTTP_204_NO_CONTENT = HttpStatus(204, "NO CONTENT");
const HttpStatus HTTP_205_RESET_CONTENT = HttpStatus(205, "RESET CONTENT");
const HttpStatus HTTP_206_PARTIAL_CONTENT = HttpStatus(206, "PARTIAL CONTENT");
const HttpStatus HTTP_207_MULTI_STATUS = HttpStatus(207, "MULTI STATUS");
const HttpStatus HTTP_300_MULTIPLE_CHOICES = HttpStatus(300, "MULTIPLE CHOICES");
const HttpStatus HTTP_301_MOVED_PERMANENTLY = HttpStatus(301, "MOVED PERMANENTLY");
const HttpStatus HTTP_302_FOUND = HttpStatus(302, "FOUND");
const HttpStatus HTTP_303_SEE_OTHER = HttpStatus(303, "SEE OTHER");
const HttpStatus HTTP_304_NOT_MODIFIED = HttpStatus(304, "NOT MODIFIED");
const HttpStatus HTTP_305_USE_PROXY = HttpStatus(305, "USE PROXY");
const HttpStatus HTTP_306_RESERVED = HttpStatus(306, "RESERVED");
const HttpStatus HTTP_307_TEMPORARY_REDIRECT = HttpStatus(307, "TEMPORARY REDIRECT");
const HttpStatus HTTP_400_BAD_REQUEST = HttpStatus(400, "BAD REQUEST");
const HttpStatus HTTP_401_UNAUTHORIZED = HttpStatus(401, "UNAUTHORIZED");
const HttpStatus HTTP_402_PAYMENT_REQUIRED = HttpStatus(402, "PAYMENT REQUIRED");
const HttpStatus HTTP_403_FORBIDDEN = HttpStatus(403, "FORBIDDEN");
const HttpStatus HTTP_404_NOT_FOUND = HttpStatus(404, "NOT FOUND");
const HttpStatus HTTP_405_METHOD_NOT_ALLOWED = HttpStatus(405, "METHOD NOT ALLOWED");
const HttpStatus HTTP_406_NOT_ACCEPTABLE = HttpStatus(406, "NOT ACCEPTABLE");
const HttpStatus HTTP_407_PROXY_AUTHENTICATION_REQUIRED = HttpStatus(407, "PROXY AUTHENTICATION REQUIRED");
const HttpStatus HTTP_408_REQUEST_TIMEOUT = HttpStatus(408, "REQUEST TIMEOUT");
const HttpStatus HTTP_409_CONFLICT = HttpStatus(409, "CONFLICT");
const HttpStatus HTTP_410_GONE = HttpStatus(410, "GONE");
const HttpStatus HTTP_411_LENGTH_REQUIRED = HttpStatus(411, "LENGTH REQUIRED");
const HttpStatus HTTP_412_PRECONDITION_FAILED = HttpStatus(412, "PRECONDITION FAILED");
const HttpStatus HTTP_413_REQUEST_ENTITY_TOO_LARGE = HttpStatus(413, "REQUEST ENTITY TOO LARGE");
const HttpStatus HTTP_414_REQUEST_URI_TOO_LONG = HttpStatus(414, "REQUEST URI TOO LONG");
const HttpStatus HTTP_415_UNSUPPORTED_MEDIA_TYPE = HttpStatus(415, "UNSUPPORTED MEDIA TYPE");
const HttpStatus HTTP_416_REQUESTED_RANGE_NOT_SATISFIABLE = HttpStatus(416, "REQUESTED RANGE NOT SATISFIABLE");
const HttpStatus HTTP_417_EXPECTATION_FAILED = HttpStatus(417, "EXPECTATION FAILED");
const HttpStatus HTTP_422_UNPROCESSABLE_ENTITY = HttpStatus(422, "UNPROCESSABLE ENTITY");
const HttpStatus HTTP_423_LOCKED = HttpStatus(423, "LOCKED");
const HttpStatus HTTP_424_FAILED_DEPENDENCY = HttpStatus(424, "FAILED DEPENDENCY");
const HttpStatus HTTP_428_PRECONDITION_REQUIRED = HttpStatus(428, "PRECONDITION REQUIRED");
const HttpStatus HTTP_429_TOO_MANY_REQUESTS = HttpStatus(429, "TOO MANY REQUESTS");
const HttpStatus HTTP_431_REQUEST_HEADER_FIELDS_TOO_LARGE = HttpStatus(431, "REQUEST HEADER FIELDS TOO LARGE");
const HttpStatus HTTP_451_UNAVAILABLE_FOR_LEGAL_REASONS = HttpStatus(451, "UNAVAILABLE FOR LEGAL REASONS");
const HttpStatus HTTP_500_INTERNAL_SERVER_ERROR = HttpStatus(500, "INTERNAL SERVER ERROR");
const HttpStatus HTTP_501_NOT_IMPLEMENTED = HttpStatus(501, "NOT IMPLEMENTED");
const HttpStatus HTTP_502_BAD_GATEWAY = HttpStatus(502, "BAD GATEWAY");
const HttpStatus HTTP_503_SERVICE_UNAVAILABLE = HttpStatus(503, "SERVICE UNAVAILABLE");
const HttpStatus HTTP_504_GATEWAY_TIMEOUT = HttpStatus(504, "GATEWAY TIMEOUT");
const HttpStatus HTTP_505_HTTP_VERSION_NOT_SUPPORTED = HttpStatus(505, "HTTP VERSION NOT SUPPORTED");
const HttpStatus HTTP_507_INSUFFICIENT_STORAGE = HttpStatus(507, "INSUFFICIENT STORAGE");
const HttpStatus HTTP_511_NETWORK_AUTHENTICATION_REQUIRED = HttpStatus(511, "NETWORK AUTHENTICATION REQUIRED");

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
                 const std::string &body = "",
                 const std::map<std::string, std::string> &headers = std::map<std::string, std::string>()
    ) : version(version),
        status(status),
        headers(headers),
        body(body) {}

    HttpResponse() {}
};
