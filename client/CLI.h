#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <set>
#include "App.h"
#include "Command.h"

class App;

class CLI
{
public:
    CLI(std::vector <std::string> args);

    void writeMessage(std::string sender, std::vector <std::string> receivers, std::string message);
    void writeInfo(std::string message);
    void writeError(std::string message);

    std::string readCommand();
    
    void run();
private:
    App* app;
    bool silent;
    std::ostringstream os;
    std::set <Command> commands;

    void executeCommand(std::string command);
};