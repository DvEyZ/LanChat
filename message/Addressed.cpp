#include "Addressed.h"

const std::string Addressed::RECEIVERS = "Addressed_receivers";

Addressed::Addressed(std::vector <std::string> r)
    :receivers(r)
{}

std::vector <std::string> Addressed::getReceivers()
{
    return receivers;
}

bool Addressed::decode(nlohmann::json json)
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

void Addressed::encode(nlohmann::json& json)
{
    json[RECEIVERS] = receivers;
}