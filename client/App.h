#pragma once

#include <boost/asio.hpp>
#include <vector>
#include <string>
#include "CLI.h"
#include "Session.h"
#include "Network.h"

class CLI;

class App
{
public:
    App(Network network, std::vector <std::string> argv);
    ~App();

    void run();
    void error(std::string what);

    CLI* getCLI();
    Session* getSession();
    Network* getNetwork();

    // commands
    void connect(std::string host);
    void identify(std::string username, std::string password);
    void sendUnicastMessage(std::vector <std::string> receivers, std::string message);
    void sendBroadcastMessage(std::string message);
    void disconnect();

private:
    CLI* cli;
    Session* session;
    Network* network;
};