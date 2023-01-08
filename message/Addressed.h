#pragma once
#include "Message.h"

class Addressed : virtual public Message
{
public:
    Addressed() {};
    Addressed(std::vector <std::string> receivers);
    virtual ~Addressed() {};
    std::vector <std::string> getReceivers();
protected:
    bool decodeContent(nlohmann::json json);
    void encodeContent(nlohmann::json& json);
    std::vector <std::string> receivers;
    const static std::string RECEIVERS;
};
