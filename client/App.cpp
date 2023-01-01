#include "App.h"

App::App(std::vector <std::string> args)
{
    session = nullptr;
    cli = new CLI(this, args);
    network = new Network(this);
}

void App::run()
{
    network_thread = std::thread(
        [this] () 
        {
            network->run();
        }
    );

    cli->run();
    
    cleanup();
}

void App::error(std::string what)
{
    cli->writeError(what);
}

void App::cleanup()
{
    network_thread.join();
}

void App::help()
{
    cli->writeInfo("LanChat client.");
}

void App::connect(std::string host)
{
    if(session != nullptr)
    {
        error("Already connected.");
        return;
    }
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
                }
                else
                {
                    error("Authentication error: " + std::to_string(message.getStatus()));
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

void App::exit()
{
    if(session != nullptr) delete session;
    cli->stop();
}

void App::onMessageReceived(ChatMessage message)
{
    cli->writeMessage(message.getSender(), message.getReceivers(), message.getMsgBody());
}