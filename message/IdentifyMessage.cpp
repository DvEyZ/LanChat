#include "IdentifyMessage.h"

const std::string IdentifyMessage::USERNAME = "Identify_username";
const std::string IdentifyMessage::PASSWORD = "Identify_password";
const std::string IdentifyMessage::MT = "identify";

IdentifyMessage::IdentifyMessage() {}

IdentifyMessage::IdentifyMessage(std::string name, std::string pass)
    :username(name), password(pass)
{}

IdentifyMessage::~IdentifyMessage() {}

std::string IdentifyMessage::getType()
{
    return MT;
}

std::string IdentifyMessage::getUsername()
{
    return username;
}

std::string IdentifyMessage::getPassword()
{
    return password;
}

void IdentifyMessage::encodeContent(nlohmann::json& json)
{
    encodeSelf(json);
}

void IdentifyMessage::encodeSelf(nlohmann::json& json)
{
    json[USERNAME] = username;
    json[PASSWORD] = password;
}

bool IdentifyMessage::decodeContent(nlohmann::json json)
{
    return decodeSelf(json);
}

bool IdentifyMessage::decodeSelf(nlohmann::json json)
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