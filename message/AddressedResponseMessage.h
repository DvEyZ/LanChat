#pragma once

#include "ResponseMessage.h"
#include "Addressed.h"

class AddressedResponseMessage : public ResponseMessage, public Addressed
{
public:
    AddressedResponseMessage(std::vector <std::string> receivers, ReadableMessageBody body, Status s = INFO);
private:
    void encodeContent(nlohmann::json& json);
    void encodeSelf(nlohmann::json& json);
    bool decodeContent(nlohmann::json json);
    bool decodeSelf(nlohmann::json json);
};
