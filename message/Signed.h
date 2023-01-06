#pragma once
#include <string>
#include <nlohmann/json.hpp>

class Signed
{
public:
    Signed(std::string sender);
    std::string getSender();
protected:
    bool decode(nlohmann::json json);
    void encode(nlohmann::json& json);
private:
    const static std::string SENDER;
    std::string sender;
};
