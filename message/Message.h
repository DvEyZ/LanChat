#pragma once

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>

#include "MessageVisitor.h"

const static std::string MESSAGE_VERSION = "1.0";

class IMessageVisitor;

class Message
{
public:
    Message();
    virtual ~Message();
    typedef std::chrono::seconds Timestamp;

    std::string getVersion();
    virtual std::string getType() = 0;
    Timestamp getTimestamp();

    std::string encode();
    bool decode(std::string message);
    
    virtual void acceptVisitor(MessageVisitor& v) = 0;
protected:
    const static std::string TYPE;
private:
    const static std::string VERSION;
    const static std::string TIMESTAMP;

    void encodeCommon(nlohmann::json& json);
    virtual void encodeContent(nlohmann::json& json) = 0;

    bool decodeCommon(nlohmann::json json);
    virtual bool decodeContent(nlohmann::json json) = 0;

    std::string version;
    Timestamp timestamp;
};
