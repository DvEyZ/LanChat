#pragma once

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>

const static std::string MESSAGE_VERSION = "1.0";

class Message
{
public:
    Message(std::string type);
    typedef std::chrono::system_clock::time_point Timestamp;

    std::string getVersion();
    std::string getType();
    Timestamp getTimestamp();

    std::string encode();
    bool decode(std::string message);
private:
    const static std::string VERSION;
    const static std::string TYPE;
    const static std::string TIMESTAMP;

    void encodeCommon(nlohmann::json& json);
    virtual void encodeContent(nlohmann::json& json) = 0;

    bool decodeCommon(nlohmann::json json);
    virtual bool decodeContent(nlohmann::json json) = 0;

    std::string version;
    std::string type;
    Timestamp timestamp;
};
