#include "App.h"

App::App(Network* _network, std::vector <std::string> args)
{
    network = _network;
    session = nullptr;
    cli = new CLI(args);
}

void App::run()
{
    std::thread network_thread(
        [this] () 
        {
            network->run();
        }
    );
    cli->run();
    
}

void App::error(std::string what)
{
    cli->writeError(what);
}

void App::connect(std::string host)
{
    network->connect(host, 
        [this] (std::shared_ptr <SocketConnection> conn)
        {
            session = new Session(this, conn);

            cli->writeInfo("Welcome to " + session->getConnection()->getRemoteIp());
        }
    );
}

void App::identify(std::string name, std::string password)
{
    if(session == nullptr)
    {
        error("You are not connected to a server.");
        return;
    }
    session->identify(name, password, 
            [this] (IdentifyResponseMessage message)
            { 
                if(message.getStatus() == IdentifyResponseMessage::Status::ok)
                {
                    cli->writeInfo("Welcome.");
                    // resume normal operation;
                }
                else
                {
                    error("Authentication error.");
                }
            }
        );
}

void App::sendUnicastMessage(std::vector <std::string> receivers, std::string message)
{
    if(session == nullptr)
    {
        error("You are not connected to a server.");
        return;
    }
    session->sendUnicastMessage(receivers, message);
    cli->writeMessage(session->getUsername(), receivers, message);
}

void App::sendBroadcastMessage(std::string message)
{
    if(session == nullptr)
    {
        error("You are not connected to a  server.");
        return;
    }
    session->sendBroadcastMessage(message);
    cli->writeMessage(session->getUsername(), {"@all"}, message);
}

void App::disconnect()
{
    if(session == nullptr)
    {
        error("You are not connected to a server.");
        return;
    }
    delete session;
    cli->writeInfo("Disconnected.");
}

void App::exit(int code)
{
    if(session != nullptr) delete session;
    delete cli;
    
    ::exit(code);
}

void App::onMessageReceived(ChatMessage message)
{
    cli->writeMessage(message.getSender(), message.getReceivers(), message.getMsgBody());
}