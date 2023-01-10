#pragma once

#include "Readable.h"
#include "Signed.h"
#include "Addressed.h"
#include <set>

class Readable;
class Signed;
class Addressed;

class SendMessage : public Readable, public Signed, public Addressed
{
public:
    const static std::string MT;
    const static std::string TYPE_TARGET;
    const static std::string TYPE_BROADCAST;
    SendMessage();
    SendMessage(std::string sender, std::vector <std::string> receivers, ReadableMessageBody body, std::string ts);
    virtual ~SendMessage();
    std::string getType();
    std::string getSendMessageType();
private:
    void encodeContent(nlohmann::json& json);
    void encodeSelf(nlohmann::json& json);
    bool decodeContent(nlohmann::json json);
    bool decodeSelf(nlohmann::json json);
    std::string type;
    const static std::string TYPE;
};
