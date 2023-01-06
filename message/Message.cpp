#include "Message.h"

unsigned int Message::getLength()
{
    return length;
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

std::vector <char> Message::encode()
{
    nlohmann::json json;
    std::string result;
    encodeCommon(json);
    encodeContent(json);

    char len_encoded[sizeof(unsigned int)];
    memmove(len_encoded, &length, sizeof(unsigned int));

    result += len_encoded;
    result += json.dump();

    return std::vector <char> (result.begin(), result.end());
}

bool Message::decode(std::string message)
{
    char* len_encoded = new char[sizeof(unsigned int)];
    len_encoded = message.substr(0, sizeof(unsigned int)).data();
    memmove(&length, len_encoded, sizeof(unsigned int));
    message.erase(0, sizeof(unsigned int));
    delete len_encoded;

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
    json["version"] = version;
    json["type"] = type;
    json["timestamp"] = timestamp.time_since_epoch().count();
}

bool Message::decodeCommon(nlohmann::json json)
{
    try
    {
        version = json["version"].get<std::string>();
        type = json["type"].get<std::string>();
        auto duration = std::chrono::seconds (json["timestamp"].get<int64_t>());
        timestamp = Timestamp(duration);
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}
