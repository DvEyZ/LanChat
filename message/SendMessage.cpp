#include "SendMessage.h"

const std::string SendMessage::MT = "send";
const std::string SendMessage::TYPE = "Send_type";
const std::string SendMessage::TYPE_TARGET = "target";
const std::string SendMessage::TYPE_BROADCAST = "broadcast";

SendMessage::SendMessage(std::string sender, std::vector <std::string> receivers, ReadableMessageBody body, std::string t)
    :Signed(sender), Addressed(receivers), Readable(body), type(t)
{}

std::string SendMessage::getSendMessageType()
{
    return type;
}

std::string SendMessage::getType()
{
    return MT;
}

void SendMessage::encodeContent(nlohmann::json& json)
{
    Signed::encodeContent(json);
    Addressed::encodeContent(json);
    Readable::encodeContent(json);
    encodeSelf(json);
}

void SendMessage::encodeSelf(nlohmann::json& json)
{
    json[TYPE] = type;
}

bool SendMessage::decodeContent(nlohmann::json json)
{
    bool s = Signed::decodeContent(json);
    bool a = Addressed::decodeContent(json);
    bool r = Readable::decodeContent(json);
    bool e = decodeSelf(json);
    return s && a && r && e;
}

bool SendMessage::decodeSelf(nlohmann::json json)
{
    try
    {
        type = json[TYPE];
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}