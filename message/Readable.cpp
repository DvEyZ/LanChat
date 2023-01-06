#include "Readable.h"

const std::string Readable::TYPE = "Readable_type";
const std::string Readable::BODY = "Readable_body";

Readable::Readable(ReadableMessageBody b)
    :body(b)
{}

ReadableMessageBody Readable::getBody()
{
    return body;
}

bool Readable::decode(nlohmann::json json)
{
    try
    {
        body = ReadableMessageBody(json[BODY].get<std::string>(), json[TYPE].get<std::string>());
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

void Readable::encode(nlohmann::json& json)
{
    json[TYPE] = body.getType();
    json[BODY] = body.getContent();
}