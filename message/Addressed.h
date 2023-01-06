#pragma once
#include <vector>
#include <string>

class Addressed
{
public:
    Addressed(std::vector <std::string> receivers);
    std::vector <std::string> getReceivers();
private:
    std::vector <std::string> receivers;
};
