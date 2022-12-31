#include "CLI.h"

CLI::CLI(std::vector <std::string> args)
    :silent(false)
{
    // currently empty
}

void CLI::run()
{
    joyterm::init();
    while(true)
    {
        std::string cmd = readCommand();
        executeCommand(cmd);
    }
}

std::string CLI::readCommand()
{
    std::string str;
    std::getline(std::cin, str);
    return str;
}

void CLI::writeMessage(std::string sender, std::vector <std::string> receivers, std::string message)
{
    std::string temp;

    temp += "[ " + sender + " ] => ";
    
    for(auto i : receivers)
    {
        temp += i + ",";
    }
    temp.erase(temp.end() - 1);

    temp += "\n" + message;


    (silent ? os : std::cout) << temp << "\n";
}

void CLI::writeError(std::string message)
{
    std::cout << joyterm::style::MStyle({joyterm::style::FgColor::red}) << 
        "ERROR: " << message 
    << joyterm::style::MStyle({joyterm::style::Style::reset}) << "\n";
}

void CLI::writeInfo(std::string message)
{
    std::cout << message << "\n";
}

void CLI::executeCommand(std::string command)
{
    std::string operation = command.substr(0, command.find(' '));
    command.erase(0, command.find(' ') + 1);
    
    std::vector <std::string> args;

    while(command.find(' ') != std::string::npos)
    {
        args.push_back(command.substr(0, command.find(' ')));
        command.erase(0, command.find(' ') + 1);    
    }

    auto op = commands.find(Command(operation, args.size()));

    if(op == commands.end())
    {
        app->error("Command not found.");
        return;
    }

    op->run(args);
}