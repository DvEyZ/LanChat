#pragma once
#include "Message.h"

class Message;

class Signed : virtual public Message
{
public:
    Signed();
    Signed(std::string sender);

    virtual ~Signed();
    std::string getSender();
protected:
    bool decodeContent(nlohmann::json json);
    void encodeContent(nlohmann::json& json);
private:
    const static std::string SENDER;
    std::string sender;
};
