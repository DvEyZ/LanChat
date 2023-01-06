#pragma once

#include <string>
#include <vector>

class ReadableMessageBody
{
public:
    std::string getType();

    std::string getText();
    std::vector <std::byte> getBin();
private:
    std::string type;
    std::vector <std::byte> content;
};
