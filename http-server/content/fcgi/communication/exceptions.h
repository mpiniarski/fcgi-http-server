#pragma once

#include "../../../exceptions.h"

class FcgiCommunicationException : public Exception {
public:
    FcgiCommunicationException(const std::string &message) : Exception(message) {}

    FcgiCommunicationException(const std::string &message, Exception &causedBy) : Exception(message, causedBy) {}
};

class FcgiCommunicationEstablishException : public FcgiCommunicationException {
public:
    FcgiCommunicationEstablishException() : FcgiCommunicationException(getMessage()) {}

    FcgiCommunicationEstablishException(Exception &causedBy) : FcgiCommunicationException(getMessage(), causedBy) {}

private:
    std::string getMessage() {
        return "Error while establishing connection with FCGI appliaction";
    }

};

class FcgiCommunicationClosedException : public FcgiCommunicationException {
public:
    FcgiCommunicationClosedException () : FcgiCommunicationException(getMessage()) {}

    FcgiCommunicationClosedException (Exception &causedBy) : FcgiCommunicationException(getMessage(), causedBy) {}

private:
    std::string getMessage() {
        return "FCGI appliaction connection has been closed";
    }

};

class FcgiCommunicationRequestSendException : public FcgiCommunicationException {
public:
    FcgiCommunicationRequestSendException() : FcgiCommunicationException(getMessage()) {}

    FcgiCommunicationRequestSendException(Exception &causedBy) : FcgiCommunicationException(getMessage(), causedBy) {}

private:
    std::string getMessage() {
        return "Error while sending FCGI request";
    }
};

class FcgiCommunicationResponseReceiveException : public FcgiCommunicationException {
public:
    FcgiCommunicationResponseReceiveException() : FcgiCommunicationException(getMessage()) {}

    FcgiCommunicationResponseReceiveException(Exception &causedBy) :
            FcgiCommunicationException(getMessage(), causedBy) {}

private:
    std::string getMessage() {
        return "Error while receiving FCGI response";
    }
};
