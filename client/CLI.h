#pragma once

#include <string>
#include <vector>
#include "App.h"

class App;

class CLI
{
public:
    CLI(std::vector <std::string> args);

    std::pair <std::string, std::string> askForAuth();
    void writeMessage(std::string sender, std::vector <std::string> receivers, std::string message);
    void writeInfo(std::string message);
    void writeError(std::string message);

    std::string readCommand();
    
    void run();
private:
    App* app;

    void executeCommand(std::string command);
};