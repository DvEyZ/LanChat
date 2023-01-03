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
    network->stop();
    network_thread.join();
}

void App::help()
{
    cli->writeInfo(
        "LanChatClient v0.1.0 - help\n"
        "Commands may be abbreviated. Parameters in {} are optional.\n\n"
        N_CONNECT " server{:port}\t\t - connect to a server.\n"
        N_DISCONNECT "\t\t\t - disconnect from a server.\n"
        N_IDENTIFY " {username} {password}\t - authenticate yourself.\n"
        N_MESSAGE " {recipient-1{,r-2,r-3}}\t - send a message to the recipients.\n"
        N_BROADCAST "\t\t\t - send a broadcast message.\n"
        N_HELP "\t\t\t\t - display this message.\n"
        N_QUIT "\t\t\t\t - exit the client.\n"
    );
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

            cli->unlock(N_IDENTIFY);
            cli->unlock(N_DISCONNECT);

            cli->prompt = session->getConnection()->getRemoteIp() + "> ";
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
                    cli->unlock(N_MESSAGE);
                    cli->unlock(N_BROADCAST);
                    
                    cli->prompt = session->getUsername() + "@" + session->getConnection()->getRemoteIp() + "> ";

                    session->listen([this] (ChatMessage m) { onMessageReceived(m); });
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
    cli->writeMessage(ChatMessage::unicast, session->getUsername(), receivers, message);
}

void App::sendBroadcastMessage(std::string message)
{
    if(session == nullptr)
    {
        error("You are not connected to a  server.");
        return;
    }
    session->sendBroadcastMessage(message);
}

void App::disconnect()
{
    if(session == nullptr)
    {
        error("You are not connected to a server.");
        return;
    }
    // this is a candidate for separate smart pointer class
    delete session;
    session = nullptr;
    cli->writeInfo("Disconnected.");

    cli->lock(N_DISCONNECT);
    cli->lock(N_IDENTIFY);
    cli->lock(N_MESSAGE);
    cli->lock(N_BROADCAST);

    cli->prompt = "> ";
}

void App::exit()
{
    if(session != nullptr) delete session;
    cli->stop();
}

void App::onMessageReceived(ChatMessage message)
{
    cli->writeMessage(message.getMessageType(), message.getSender(), message.getReceivers(), message.getMsgBody());
}