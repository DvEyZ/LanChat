#pragma once

#include "Message.h"
#include "Readable.h"
#include "Addressed.h"

class ResponseMessage : public Message, public Readable, public Addressed
{
public:
    const static std::string MT;
    enum Status
    {
        OK = 0,
        FAIL = 100,
        MALFORMED_MESSAGE = 101,
        BAD_DATA = 102,
        DENIED = 103
    };
    ResponseMessage();
    ResponseMessage(std::vector <std::string> receivers, ReadableMessageBody body, Status status = OK);
    Status getStatus();
    bool ok();
private:
    void encodeContent(nlohmann::json& json);
    void encodeSelf(nlohmann::json& json);
    bool decodeContent(nlohmann::json json);
    bool decodeSelf(nlohmann::json json);
    Status status;

    const static std::string STATUS;
};
