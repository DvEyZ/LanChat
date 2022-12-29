#pragma once

#include <boost/asio.hpp>
#include <vector>
#include <string>
#include <thread>
#include "CLI.h"
#include "Session.h"
#include "Network.h"

class CLI;
class Network;
class Session;

class App
{
public:
    App(Network* network, std::vector <std::string> args);
    ~App();

    void run();
    void error(std::string what);

    // commands
    void connect(std::string host);
    void identify();
    void sendUnicastMessage(std::vector <std::string> receivers, std::string message);
    void sendBroadcastMessage(std::string message);
    void disconnect();

    // events
    void onConnected();
    void onIdentified();
    void onMessageSent();
    void onDisconnected();
private:
    CLI* cli;
    Session* session;
    Network* network;
};