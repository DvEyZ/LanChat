#pragma once
#include <string>
#include <nlohmann/json.hpp>

class Signed
{
public:
    Signed() {};
    Signed(std::string sender);
    std::string getSender();
protected:
    bool decodeSelf(nlohmann::json json);
    void encodeSelf(nlohmann::json& json);
private:
    const static std::string SENDER;
    std::string sender;
};
