#pragma once

#include <string>
#include <vector>

class ReadableMessageBody
{
public:
    ReadableMessageBody(std::string str, std::string type = "text");
    std::string getType();
    std::string getContent();  
private:
    std::string type;
    std::string content;
};
