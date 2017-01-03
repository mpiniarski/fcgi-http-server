#pragma once

/*
 * Listening socket file number
 */
#define FCGI_LISTENSOCK_FILENO 0

/*
 * Number of bytes in a FCGI_Header.  Future versions of the protocol
 * will not reduce this number.
 */
#define FCGI_HEADER_LEN 8

/*
 * Value for version component of FCGI_Header
 */
#define FCGI_VERSION_1 1

/*
 * Values for type component of FCGI_Header
 */
#define FCGI_BEGIN_REQUEST 1
#define FCGI_ABORT_REQUEST 2
#define FCGI_END_REQUEST 3
#define FCGI_PARAMS 4
#define FCGI_STDIN 5
#define FCGI_STDOUT 6
#define FCGI_STDERR 7
#define FCGI_DATA 8
#define FCGI_GET_VALUES 9
#define FCGI_GET_VALUES_RESULT 10
#define FCGI_UNKNOWN_TYPE 11
#define FCGI_MAXTYPE (FCGI_UNKNOWN_TYPE)

/*
 * Value for requestId component of FCGI_Header
 */
#define FCGI_NULL_REQUEST_ID 0

/*
 * Mask for flags component of FCGI_BeginRequestBody
 */
#define FCGI_KEEP_CONN 1

/*
 * Values for role component of FCGI_BeginRequestBody
 */
#define FCGI_RESPONDER 1
#define FCGI_AUTHORIZER 2
#define FCGI_FILTER 3

/*
 * Values for protocolStatus component of FCGI_EndRequestBody
 */
#define FCGI_REQUEST_COMPLETE 0
#define FCGI_CANT_MPX_CONN 1
#define FCGI_OVERLOADED 2
#define FCGI_UNKNOWN_ROLE 3

/*
 * Variable names for FCGI_GET_VALUES / FCGI_GET_VALUES_RESULT records
 */
#define FCGI_MAX_CONNS "FCGI_MAX_CONNS"
#define FCGI_MAX_REQS "FCGI_MAX_REQS"
#define FCGI_MPXS_CONNS "FCGI_MPXS_CONNS"

struct FCGI_Header {
    unsigned char version;
    unsigned char type;
    uint16_t requestId;
    uint16_t contentLength;
    uint8_t paddingLength;
    unsigned char reserved;

    FCGI_Header(unsigned char type, uint16_t requestId, uint16_t contentLength, uint8_t paddingLength) :
            version(FCGI_VERSION_1),
            type(type),
            paddingLength(paddingLength) {
        this->requestId = htons(requestId);
        this->contentLength = htons(contentLength);
    }

    FCGI_Header(void *message) {
        *this = *(FCGI_Header *) message;
        this->contentLength = htons(this->contentLength);
        this->requestId = htons(this->requestId);
    }
};

struct FCGI_BeginRequestBody {
    uint16_t role;
    unsigned char flags;
    unsigned char reserved[5];

    FCGI_BeginRequestBody(uint16_t role, unsigned char flags) :
            flags(flags) {
        this->role = htons(role);
    }
};


struct FCGI_EndRequestBody {
    uint32_t appStatus;
    uint8_t protocolStatus;
    unsigned char reserved[3];

    FCGI_EndRequestBody(void *message) {
        *this = *(FCGI_EndRequestBody *) message;
        this->appStatus = htonl(this->appStatus);
    }
};


struct FCGI_UnknownTypeBody {
    unsigned char type;
    unsigned char reserved[7];
};

