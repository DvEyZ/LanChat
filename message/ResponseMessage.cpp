#include "ResponseMessage.h"

const std::string ResponseMessage::STATUS = "Response_status";
const std::string ResponseMessage::MT = "response";


ResponseMessage::ResponseMessage(ReadableMessageBody body, Status s)
    :Readable(body), status(s)
{}

std::string ResponseMessage::getType()
{
    return MT;
}

void ResponseMessage::encodeContent(nlohmann::json& json)
{
    Readable::encodeContent(json);
    encodeSelf(json);
}

bool ResponseMessage::decodeContent(nlohmann::json json)
{
    bool r = Readable::decodeContent(json);
    bool e = decodeSelf(json);
    return r && e;
}

void ResponseMessage::encodeSelf(nlohmann::json& json)
{
    json[STATUS] = status;
}

bool ResponseMessage::decodeSelf(nlohmann::json json)
{
    try
    {
        status = json[STATUS];
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

ResponseMessage::Status ResponseMessage::getStatus()
{
    return status;
}

bool ResponseMessage::ok()
{
    return status < 300;
}