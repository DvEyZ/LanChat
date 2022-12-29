#include "App.h"

App::App(Network* _network, std::vector <std::string> args)
{
    network = _network;
    session = nullptr;
    cli = new CLI(args);
}

void App::run()
{
    cli->run();
    std::thread network_thread(
        [this] () 
        {
            network->run();
        }
    );
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

void App::identify()
{
    if(session == nullptr)
    {
        error("You are not connected to a server.");
        return;
    }
    auto auth = cli->askForAuth();
    session->identify(auth.first, auth.second, 
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
    
}