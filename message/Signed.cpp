#include "Signed.h"

const std::string Signed::SENDER = "Signed_sender";

Signed::Signed(std::string s)
    :sender(s)
{}

std::string Signed::getSender()
{
    return sender;
}

bool Signed::decodeSelf(nlohmann::json json)
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

void Signed::encodeSelf(nlohmann::json& json)
{
    json[SENDER] = sender;
}