#pragma once

#include "Message.h"
#include "Readable.h"
#include "Addressed.h"

class ResponseMessage : public Readable
{
public:
    const static std::string MT;
    enum Status
    {
        INFO = 100,                 // Server sends information. Default response.
        DONE = 200,                 // Server has taken the action successfuly.
        FAIL = 300,                 // Unspecified failure. Prefer to use other codes, use this only if no other code matches.
        MALFORMED_MESSAGE = 301,    // Malformed message. Use this if decoding of message fails.
        BAD_DATA = 302,             // Bad data. Use this if decoding succeeds, but the data itself either is either malformed, or makes no sense together.
        DENIED = 303,               // Denied. Use this if data is OK, but the server denies it, for any reason.
    };
    ResponseMessage() {};
    ResponseMessage(ReadableMessageBody body, Status status = INFO);

    virtual ~ResponseMessage() {};
    std::string getType();
    Status getStatus();
    bool ok();
protected:
    void encodeContent(nlohmann::json& json);
    void encodeSelf(nlohmann::json& json);
    bool decodeContent(nlohmann::json json);
    bool decodeSelf(nlohmann::json json);
    Status status;

    const static std::string STATUS;
};
