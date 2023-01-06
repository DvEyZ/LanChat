#include "ResponseMessage.h"

const std::string ResponseMessage::STATUS = "Response_status";
const std::string ResponseMessage::MT = "response";

ResponseMessage::ResponseMessage()
    :Message(MT)
{}

ResponseMessage::ResponseMessage(std::vector <std::string> receivers, ReadableMessageBody body, Status s)
    :Message(MT), Addressed(receivers), Readable(body), status(s)
{}

void ResponseMessage::encodeContent(nlohmann::json& json)
{
    Addressed::encodeSelf(json);
    Readable::encodeSelf(json);
    encodeSelf(json);
}

bool ResponseMessage::decodeContent(nlohmann::json json)
{
    bool a = Addressed::decodeSelf(json);
    bool r = Readable::decodeSelf(json);
    bool e = decodeSelf(json);
    return a && r && e;
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
    return status == ResponseMessage::OK;
}