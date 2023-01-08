#include "Message.h"

const std::string Message::TYPE = "Message_type";
const std::string Message::VERSION = "Message_version";
const std::string Message::TIMESTAMP = "Message_timestamp";

Message::Message()
{
    version = MESSAGE_VERSION;
    auto tse = std::chrono::system_clock::now().time_since_epoch();
    timestamp = std::chrono::duration_cast <std::chrono::seconds> (tse);
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
    json[TYPE] = getType();
    json[TIMESTAMP] = timestamp.count();
}

bool Message::decodeCommon(nlohmann::json json)
{
    try
    {
        if(json[TYPE] != getType())
            return false;
        version = json[VERSION].get<std::string>();
        // type = json[TYPE].get<std::string>();
        auto duration = (json[TIMESTAMP].get<int64_t>());
        timestamp = std::chrono::seconds(duration);
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}
