#pragma once
#include "ReadableMessageBody.h"
#include <nlohmann/json.hpp>

class Readable
{
public:
    Readable(ReadableMessageBody b);
    ReadableMessageBody getBody();
protected:
    bool decode(nlohmann::json json);
    void encode(nlohmann::json& json);
private:
    ReadableMessageBody body;
    const static std::string TYPE;
    const static std::string BODY;
};
