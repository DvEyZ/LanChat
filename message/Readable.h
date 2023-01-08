#pragma once
#include "ReadableMessageBody.h"
#include "Message.h"

class Readable : virtual public Message
{
public:
    Readable() {};
    Readable(ReadableMessageBody b);
    virtual ~Readable() {};
    ReadableMessageBody getBody();
protected:
    bool decodeContent(nlohmann::json json);
    void encodeContent(nlohmann::json& json);
private:
    ReadableMessageBody body;
    const static std::string BODY_TYPE;
    const static std::string BODY;
};
