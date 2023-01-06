#pragma once

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>

class Message
{
public:
    typedef std::chrono::time_point <std::chrono::seconds> Timestamp;
    unsigned int getLength();
    std::string getVersion();
    std::string getType();
    Timestamp getTimestamp();

    std::vector <char> encode();
    bool decode(std::string message);
private:
    void encodeCommon(nlohmann::json& json);
    virtual void encodeContent(nlohmann::json& json) = 0;

    bool decodeCommon(nlohmann::json json);
    virtual bool decodeContent(nlohmann::json json) = 0;

    unsigned int length;
    std::string version;
    std::string type;
    Timestamp timestamp;
};
