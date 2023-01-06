#include "Message.h"

const std::string Message::TYPE = "Message_type";
const std::string Message::VERSION = "Message_version";
const std::string Message::TIMESTAMP = "Message_timestamp";

Message::Message(std::string t)
{
    type = t;
    version = MESSAGE_VERSION;
    timestamp = std::chrono::system_clock::now().time_since_epoch();
}

std::string Message::getType()
{
    return type;
}

std::string Message::getVersion()
{
    return version;
}

Message::Timestamp Message::getTimestamp()
{
    return timestamp;
}

std::string Message::encode()
{
    nlohmann::json json;
    encodeCommon(json);
    encodeContent(json);
    return json.dump();
}

bool Message::decode(std::string message)
{
    nlohmann::json json;
    try
    {
        json = nlohmann::json::parse(message);
    }
    catch(const std::exception& e)
    {
        return false;
    }

    bool dcm = decodeCommon(json);
    bool dcc = decodeContent(json);

    return dcm && dcc;
}

void Message::encodeCommon(nlohmann::json& json)
{
    json[VERSION] = version;
    json[TYPE] = type;
    json[TIMESTAMP] = timestamp.count();
}

bool Message::decodeCommon(nlohmann::json json)
{
    try
    {
        version = json[VERSION].get<std::string>();
        type = json[TYPE].get<std::string>();
        auto duration = (json[TIMESTAMP].get<int64_t>());
        timestamp = std::chrono::system_clock::duration(duration);
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}
