#pragma once

#include <joyterm.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <mutex>
#include "App.h"
#include "Command.h"

static std::mutex cout_mutex;

class App;


class CLI
{
public:
    CLI(App* app, std::vector <std::string> args);

    void writeMessage(std::string sender, std::vector <std::string> receivers, std::string message);
    void writeInfo(std::string message);
    void writeError(std::string message);

    std::string readCommand();
    std::string ask(std::string what);
        
    void silent();
    void nosilent();

    void run();
    void stop();
private:
    App* app;
    bool silent_flag;
    bool running;
    std::ostringstream os;
    std::vector <Command> commands;

    void executeCommand(std::string command);
    void write(std::string what);
    Command findUniqueCommand(std::string name, int args);
};