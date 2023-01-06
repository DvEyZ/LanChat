#pragma once
#include "ReadableMessageBody.h"
#include <nlohmann/json.hpp>

class Readable
{
public:
    Readable() {};
    Readable(ReadableMessageBody b);
    ReadableMessageBody getBody();
protected:
    bool decodeSelf(nlohmann::json json);
    void encodeSelf(nlohmann::json& json);
private:
    ReadableMessageBody body;
    const static std::string BODY_TYPE;
    const static std::string BODY;
};
