#pragma once
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class Addressed
{
public:
    Addressed(std::vector <std::string> receivers);
    std::vector <std::string> getReceivers();
protected:
    bool decode(nlohmann::json json);
    void encode(nlohmann::json& json);
private:
    std::vector <std::string> receivers;
    const static std::string RECEIVERS;
};
