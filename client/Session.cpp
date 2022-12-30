#include "Session.h"

Session::Session(App* _app, std::shared_ptr <Connection> _connection)
    :app(_app), connection(_connection)
{
    connection->setErrorCallback(
        [this] (SocketConnectionError error)
        {
            app->error(error.what());
        }
    );
}

std::shared_ptr <Connection> Session::getConnection()
{
    return connection;
}

void Session::identify(std::string username, std::string password, std::function < void(IdentifyResponseMessage) > callback)
{
    IdentifyMessage m(username, password);
    connection->write(m.encodeMessage(), 
        [this, callback, m] () mutable
        {
            connection->read(
                [this, callback, m] (std::vector <char> data) mutable
                {
                    IdentifyResponseMessage r;
                    if(r.decodeAll(data))
                    {
                        this->username = m.getUsername();
                        callback(r);
                    }
                    else
                    {
                        app->error("Malformed message from server.");
                    }
                }
            );
        }
    );
}

void Session::sendUnicastMessage(std::vector <std::string> receivers, std::string message)
{
    ChatMessage m(ChatMessage::MessageType::unicast, getUsername(), receivers, message);
    connection->write(m.encodeMessage(), [] () {});
}

void Session::sendBroadcastMessage(std::string message)
{
    ChatMessage m(ChatMessage::MessageType::broadcast, getUsername(), {}, message);
    connection->write(m.encodeMessage(), [] () {});
}

void Session::recvMessage(std::function <void(ChatMessage)> callback)
{
    connection->read(
        [this, callback] (std::vector <char> data)
        {
            ChatMessage m;
            if(m.decodeAll(data))
                callback(m);
            else
                app->error("Malformed message.");
        }
    );
}

std::string Session::getUsername()
{
    return username;
}