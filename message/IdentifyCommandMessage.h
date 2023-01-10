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
    void encodeSelf(nlohmann::json& json);
    bool decodeSelf(nlohmann::json json);

    std::string username;
    std::string password;

    const static std::string USERNAME;
    const static std::string PASSWORD;
};
