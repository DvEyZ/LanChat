#pragma once

#include "Message.h"
#include "Readable.h"
#include "Signed.h"
#include "Addressed.h"

class SendMessage : public Message, public Readable, public Signed, public Addressed
{
    const static std::string MT;
    SendMessage(std::string sender, std::vector <std::string> receivers, ReadableMessageBody body, std::string type);
    std::string getType();
private:
    void encodeContent(nlohmann::json& json);
    void encodeSelf(nlohmann::json& json);
    bool decodeContent(nlohmann::json json);
    bool decodeSelf(nlohmann::json json);
    std::string type;
    const static std::string TYPE;
    const static std::string TYPE_TARGET;
    const static std::string TYPE_BROADCAST;
};
