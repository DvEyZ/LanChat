#include "AddressedResponseMessage.h"

AddressedResponseMessage::AddressedResponseMessage(std::vector <std::string> receivers, ReadableMessageBody body, Status s)
    :Addressed(receivers), ResponseMessage(body, s)
{}

std::string AddressedResponseMessage::getType()
{
    return MT;
}

void AddressedResponseMessage::encodeContent(nlohmann::json& json)
{
    Addressed::encodeContent(json);
    ResponseMessage::encodeContent(json);
}

bool AddressedResponseMessage::decodeContent(nlohmann::json json)
{
    bool a = Addressed::decodeContent(json);
    bool r = ResponseMessage::decodeContent(json);
    return a && r;
}