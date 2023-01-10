#pragma once

#include "ResponseMessage.h"
#include "Addressed.h"
#include <set>

class ResponseMessage;
class Addressed;

class AddressedResponseMessage : public ResponseMessage, public Addressed
{
public:
    AddressedResponseMessage();
    AddressedResponseMessage(std::vector <std::string> receivers, ReadableMessageBody body, Status s = INFO);
    virtual ~AddressedResponseMessage();

    std::string getType();
private:
    void encodeContent(nlohmann::json& json);
    bool decodeContent(nlohmann::json json);
};
