#pragma once

#include <map>
#include "Signed.h"

class CommandMessage : Signed
{
public:
    const static std::string MT;
    CommandMessage();
    CommandMessage(std::string sender, std::string action, std::map <std::string, std::string> params);
    virtual ~CommandMessage();

    std::string getType();
    std::string getAction();
    std::map <std::string, std::string> getParams();
    std::string getParam(std::string w);
protected:
    void encodeContent(nlohmann::json& json);
    void encodeSelf(nlohmann::json& json);
    bool decodeContent(nlohmann::json json);
    bool decodeSelf(nlohmann::json json);

private:
    std::string action;
    std::map <std::string, std::string> params;

    const static std::string ACTION;
    const static std::string PARAMS;
};
