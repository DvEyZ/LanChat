#pragma once

#include "Message.h"

class IdentifyMessage : public Message
{
public:
    const static std::string MT;
    IdentifyMessage() {};
    IdentifyMessage(std::string name, std::string pass);

    virtual ~IdentifyMessage() {};
    std::string getType();
    std::string getUsername();
    std::string getPassword();
    void acceptVisitor(IMessageVisitor& v);
private:
    void encodeContent(nlohmann::json& json);
    void encodeSelf(nlohmann::json& json);
    bool decodeContent(nlohmann::json json);
    bool decodeSelf(nlohmann::json json);

    std::string username;
    std::string password;

    const static std::string USERNAME;
    const static std::string PASSWORD;
};
