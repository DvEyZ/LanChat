#include "IdentifyCommandMessage.h"

const std::string IdentifyCommandMessage::ACTION_NAME = "identify";
const std::string IdentifyCommandMessage::USERNAME = "Identify_username";
const std::string IdentifyCommandMessage::PASSWORD = "Identify_password";

IdentifyCommandMessage::IdentifyCommandMessage() {}

IdentifyCommandMessage::IdentifyCommandMessage(std::string name, std::string pass)
    :CommandMessage(name, ACTION_NAME, { {USERNAME, name }, {PASSWORD, pass} })
{}

IdentifyCommandMessage::~IdentifyCommandMessage() {}

std::string IdentifyCommandMessage::getUsername()
{
    return getParam(USERNAME);
}

std::string IdentifyCommandMessage::getPassword()
{
    return getParam(PASSWORD);
}

void IdentifyCommandMessage::encodeContent(nlohmann::json& json)
{
    CommandMessage::encodeContent(json);
    encodeSelf(json);
}

bool IdentifyCommandMessage::decodeContent(nlohmann::json json)
{
    bool c = CommandMessage::decodeContent(json);
    bool s = decodeSelf(json);
    return c && s;
}

bool IdentifyCommandMessage::decodeSelf(nlohmann::json json)
{
    return json[ACTION] == ACTION_NAME;
}