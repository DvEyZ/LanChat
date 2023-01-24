#include "CommandMessage.h"

const std::string MT = "command";

CommandMessage::CommandMessage()
{}

CommandMessage::CommandMessage(std::string sender, std::string a, std::map <std::string, std::string> p)
    :Signed(sender), action(a), params(p)
{}

CommandMessage::~CommandMessage()
{}

std::string CommandMessage::getType()
{
    return MT;
}

std::string CommandMessage::getAction()
{
    return action;
}

std::map <std::string, std::string> CommandMessage::getParams()
{
    return params;
}

std::string CommandMessage::getParam(std::string w)
{
    return params.at(w);
}

void CommandMessage::acceptVisitor(MessageVisitor &v)
{
    v.visitCommandMessage(this);
}

void CommandMessage::encodeContent(nlohmann::json& json)
{
    Signed::encodeContent(json);
    encodeSelf(json);
}

void CommandMessage::encodeSelf(nlohmann::json& json)
{
    json[ACTION] = action;
    json[PARAMS] = params;
}

bool CommandMessage::decodeContent(nlohmann::json json)
{
    bool s = Signed::decodeContent(json);
    bool f = decodeSelf(json);

    return s && f;
}

bool CommandMessage::decodeSelf(nlohmann::json json)
{
    try
    {
        action = json[ACTION].get<std::string>();
        params = json[PARAMS].get<std::map<std::string,std::string>>();
    }
    catch (std::exception& e)
    {
        return false;
    }
    return true;
}