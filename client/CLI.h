#pragma once

#include <string>
#include <vector>
#include "App.h"

class App;

class CLI
{
public:
    void writeMessage(std::string message);
    void writeError(std::string message);

    void readCommand();
    

    void run();
private:
    App* app;

    void executeCommand(std::string command);
};