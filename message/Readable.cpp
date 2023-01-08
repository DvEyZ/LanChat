#include "Readable.h"

const std::string Readable::BODY_TYPE = "Readable_type";
const std::string Readable::BODY = "Readable_body";

Readable::Readable() {}

Readable::Readable(ReadableMessageBody b)
    :body(b)
{}

Readable::~Readable() {}

ReadableMessageBody Readable::getBody()
{
    return body;
}

bool Readable::decodeContent(nlohmann::json json)
{
    try
    {
        body = ReadableMessageBody(json[BODY].get<std::string>(), json[BODY_TYPE].get<std::string>());
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

void Readable::encodeContent(nlohmann::json& json)
{
    json[BODY_TYPE] = body.getType();
    json[BODY] = body.getContent();
}