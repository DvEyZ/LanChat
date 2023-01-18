#pragma once

#include "CommandMessage.h"

class IdentifyCommandMessage : public CommandMessage
{
public:
    IdentifyCommandMessage();
    IdentifyCommandMessage(std::string name, std::string pass);

    virtual ~IdentifyCommandMessage();
    std::string getUsername();
    std::string getPassword();

private:
    void encodeContent(nlohmann::json& json);
    void encodeSelf(nlohmann::json& json);
    bool decodeContent(nlohmann::json json);
    bool decodeSelf(nlohmann::json json);

    const static std::string ACTION_NAME;
    const static std::string USERNAME;
    const static std::string PASSWORD;
};
