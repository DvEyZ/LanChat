#include "Command.h"

Command::Command(std::string _name, int _arg_num)
    :name(_name), arg_num(_arg_num)
{}

Command::Command(std::string _name, int _arg_num, std::function <void(std::vector <std::string> args)> _op)
    :name(_name), arg_num(_arg_num), op(_op)
{}

void Command::run(std::vector <std::string> args) const
{
    op(args);
}

bool Command::operator <(Command command)
{
    if(this->name != command.name) return this->name < command.name;
    else return this->arg_num < command.arg_num;
}