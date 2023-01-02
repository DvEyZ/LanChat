#pragma once

#include <string>
#include <vector>
#include <functional>

class Command
{
public:
    Command(std::string name, int arg_num);
    Command(std::string name, int arg_num, std::function <void(std::vector <std::string> args)> op);
    const std::string name;
    const int arg_num;
    
    void run(std::vector <std::string> args) const;
private:
    const std::function <void(std::vector <std::string> args)> op;
};