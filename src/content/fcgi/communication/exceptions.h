#pragma once

#include "../../../exceptions.h"

class FcgiCommunicationException : public Exception {
public:
    FcgiCommunicationException(const std::string &message) : Exception(message) {}
};

class FcgiCommunicationEstablishException : public FcgiCommunicationException {
public:
    FcgiCommunicationEstablishException() : FcgiCommunicationException ("Error while establishing connection with FCGI appliaction") {}
};

class FcgiCommunicationRequestSendException : public FcgiCommunicationException {
public:
    FcgiCommunicationRequestSendException() : FcgiCommunicationException("Error while sending FCGI request") {}
};

class FcgiCommunicationResponseReceiveException : public FcgiCommunicationException {
public:
    FcgiCommunicationResponseReceiveException() : FcgiCommunicationException("Error while receiving FCGI response") {}
};
