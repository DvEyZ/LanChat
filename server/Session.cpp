//FINISHEDD

#include "Session.h"

Session::Session(boost::shared_ptr<Connection> _connection, Chat* _chat)
    :connection(_connection), chat(_chat)
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

void Session::readIdentification()
{
    connection->read(std::bind(&Session::onReadIdentification, shared_from_this(), std::placeholders::_1));
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
    connection->write(resp.encodeMessage(), std::bind(&Session::onWriteIdentification, shared_from_this(), resp.getStatus()));
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
    connection->write(message.encodeMessage(), std::bind(&Session::onWriteMessage, shared_from_this()));
}

void Session::onWriteMessage()
{
    recentMsgWrite.pop();
    if(!recentMsgWrite.empty())
    {
        writeMessage(recentMsgWrite.front());
    }
}

void Session::onError(const boost::system::error_code& error)
{
    chat->log(error.message());
    chat->leave(shared_from_this());
}