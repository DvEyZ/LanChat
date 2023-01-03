#pragma once

#include <joyterm.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <set>
#include <mutex>
#include "App.h"
#include "Command.h"

#define N_CONNECT "connect"
#define N_DISCONNECT "disconnect"
#define N_IDENTIFY "identify"
#define N_MESSAGE "message"
#define N_BROADCAST "broadcast"
#define N_HELP "help"
#define N_QUIT "quit"

static std::mutex cout_mutex;

class App;
class ChatMessage;

class CLI
{
public:
    CLI(App* app, std::vector <std::string> args);

    void writeMessage(int type, std::string sender, std::vector <std::string> receivers, std::string message);
    void writeInfo(std::string message);
    void writeError(std::string message);

    std::string readCommand();
    std::string ask(std::string what);

    void lock(std::string name);
    void unlock(std::string name);

    void run();
    void stop();

    std::string prompt;
private:
    App* app;
    bool running;
    std::ostringstream os;

    std::set <std::string> locked;
    std::vector <Command> commands;

    void executeCommand(std::string command);
    void write(std::string what);
    Command findUniqueCommand(std::string name, int args);
};