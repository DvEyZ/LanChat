//FINISHEDD

#include "Session.h"

std::set <std::shared_ptr <Session>> Session::awaiting_for_identification;

Session::Session(std::shared_ptr<Connection> _connection, Chat* _chat, std::shared_ptr <Logger>  _logger)
    :connection(_connection), chat(_chat), logger(_logger)
{

}

Session::~Session()
{}

std::string Session::getUser()
{
    return user;
}

void Session::run()
{
    Session::awaiting_for_identification.emplace(shared_from_this());
    readIdentification();
}

void Session::main()
{
    readMessage();
}

void Session::readMessage()
{
    try
    {
        connection->read(std::bind(&Session::onReadMessage, shared_from_this(), std::placeholders::_1));
    }
    catch(ConnectionException e)
    {
        onError(e);
    }
}

void Session::onReadMessage(std::vector <char> message)
{
    ChatMessage m;
    if(m.decodeAll(message))
    {
        chat->messageIncoming(m);
        main();
    }
}

void Session::readIdentification()
{
    try
    {
        connection->read(std::bind(&Session::onReadIdentification, shared_from_this(), std::placeholders::_1));
    }
    catch(ConnectionException e)
    {
        onError(e);
    }
}

void Session::onReadIdentification(std::vector <char> message)
{
    IdentifyMessage id;
    if(id.decodeAll(message))
    {
        identify(id);
    }
    else
    {
        writeIdentification(IdentifyResponseMessage::Status::auth_failed_malformed);
    }
}

void Session::identify(IdentifyMessage id)
{
    user = id.getUsername();
    IdentifyResponseMessage::Status status;
    status = chat->auth->authenticate(id);
    if(status == IdentifyResponseMessage::Status::ok)
        status = chat->auth->permitConnection(shared_from_this());
    
    IdentifyResponseMessage resp(status);

    writeIdentification(resp);
}

void Session::writeIdentification(IdentifyResponseMessage resp)
{
    try 
    {
        connection->write(resp.encodeMessage(), std::bind(&Session::onWriteIdentification, shared_from_this(), resp.getStatus()));
        // error location, bind fails
    }
    catch(ConnectionException e)
    {
        onError(e);
    }
}

void Session::onWriteIdentification(IdentifyResponseMessage::Status status)
{
    if(status = IdentifyResponseMessage::Status::ok)
    {
        chat->join(shared_from_this());
        Session::awaiting_for_identification.erase(shared_from_this());
        main();
    }
    else
    {
        Session::awaiting_for_identification.erase(shared_from_this());
    }
}

void Session::postMessage(ChatMessage message)
{
    bool idle = recentMsgWrite.empty();
    recentMsgWrite.push(message);
    if(idle)
    {
        writeMessage(recentMsgWrite.front());
    }
}

void Session::writeMessage(ChatMessage message)
{
    try
    {
        connection->write(message.encodeMessage(), std::bind(&Session::onWriteMessage, shared_from_this()));
    }
    catch(ConnectionException e)
    {
        onError(e);
    }
}

void Session::onWriteMessage()
{
    recentMsgWrite.pop();
    if(!recentMsgWrite.empty())
    {
        writeMessage(recentMsgWrite.front());
    }
}

void Session::onError(ConnectionException& error)
{
    logger->log(error.what());
    chat->leave(shared_from_this());
}

std::shared_ptr <Connection> Session::getConnection()
{
    return connection;
}