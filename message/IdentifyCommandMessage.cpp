#include "IdentifyCommandMessage.h"

const std::string IdentifyCommandMessage::USERNAME = "Identify_username";
const std::string IdentifyCommandMessage::PASSWORD = "Identify_password";

IdentifyCommandMessage::IdentifyCommandMessage() {}

IdentifyCommandMessage::IdentifyCommandMessage(std::string name, std::string pass)
    :username(name), password(pass)
{}

IdentifyCommandMessage::~IdentifyCommandMessage() {}

std::string IdentifyCommandMessage::getUsername()
{
    return username;
}

std::string IdentifyCommandMessage::getPassword()
{
    return password;
}

void IdentifyCommandMessage::encodeContent(nlohmann::json& json)
{
    encodeSelf(json);
}

void IdentifyCommandMessage::encodeSelf(nlohmann::json& json)
{
    json[USERNAME] = username;
    json[PASSWORD] = password;
}

bool IdentifyCommandMessage::decodeContent(nlohmann::json json)
{
    return decodeSelf(json);
}

bool IdentifyCommandMessage::decodeSelf(nlohmann::json json)
{
    try
    {
        username = json[USERNAME];
        password = json[PASSWORD];
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;   
}