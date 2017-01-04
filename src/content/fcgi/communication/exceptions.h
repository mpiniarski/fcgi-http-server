#pragma once

#include "../../../exceptions.h"

class FcgiCommunicationException : public Exception {
public:
    FcgiCommunicationException(const std::string &message) : Exception(message) {}
};

class FcgiCommunicationEstablishException : public Exception {
public:
    FcgiCommunicationEstablishException() : Exception("Error while establishing connection with FCGI appliaction") {}
};

class FcgiCommunicationRequestSendException : public Exception {
public:
    FcgiCommunicationRequestSendException() : Exception("Error while sending FCGI request") {}
};

class FcgiCommunicationResponseReceiveException : public Exception {
public:
    FcgiCommunicationResponseReceiveException() : Exception("Error while receiving FCGI response") {}
};
