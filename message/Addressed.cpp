#include "Addressed.h"

const std::string Addressed::RECEIVERS = "Addressed_receivers";

Addressed::Addressed() {}

Addressed::Addressed(std::vector <std::string> r)
    :receivers(r)
{}

Addressed::~Addressed() {}

std::vector <std::string> Addressed::getReceivers()
{
    return receivers;
}

bool Addressed::decodeContent(nlohmann::json json)
{
    try
    {
        receivers = json[RECEIVERS].get<std::vector <std::string>>();
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

void Addressed::encodeContent(nlohmann::json& json)
{
    json[RECEIVERS] = receivers;
}