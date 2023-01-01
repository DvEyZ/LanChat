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
    App(std::vector <std::string> args);

    void run();
    void error(std::string what);

    // commands
    void help();
    void connect(std::string host);
    void identify(std::string name, std::string password);
    void sendUnicastMessage(std::vector <std::string> receivers, std::string message);
    void sendBroadcastMessage(std::string message);
    void disconnect();
    void exit();

    // events
    void onMessageReceived(ChatMessage message);

    
private:
    CLI* cli;
    Session* session;
    Network* network;

    std::thread network_thread;
    
    void cleanup();
};