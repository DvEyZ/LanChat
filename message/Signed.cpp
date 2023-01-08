#include "Signed.h"

const std::string Signed::SENDER = "Signed_sender";

Signed::Signed(std::string s)
    :sender(s)
{}

std::string Signed::getSender()
{
    return sender;
}

bool Signed::decodeContent(nlohmann::json json)
{
    try
    {
        sender = json[SENDER].get<std::string>();
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

void Signed::encodeContent(nlohmann::json& json)
{
    json[SENDER] = sender;
}