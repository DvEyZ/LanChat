#pragma once
#include <string>

class Signed
{
public:
    Signed(std::string sender);
    std::string getSender();
private:
    std::string sender;
};
