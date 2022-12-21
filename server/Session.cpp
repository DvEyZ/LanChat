//FINISHEDD

#include "Session.h"

std::set <std::shared_ptr <Session>> Session::awaiting_for_identification;

Session::Session(std::shared_ptr<Connection> _connection, Chat* _chat, std::shared_ptr <Logger>  _logger)
    :connection(_connection), chat(_chat), logger(_logger)
{
    connection->setErrorCallback([this] (SocketConnectionError err) mutable { onError(err); });
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
        connection->read([this] (std::vector <char> message) {onReadMessage(message);});
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
    connection->read([this] (std::vector <char> message) {onReadIdentification(message);});
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
    connection->write(resp.encodeMessage(), [this, resp] () mutable { onWriteIdentification(resp.getStatus()); });
}

void Session::onWriteIdentification(IdentifyResponseMessage::Status status)
{
    if(status == IdentifyResponseMessage::Status::ok)
    {
        chat->join(shared_from_this());
    }
    Session::awaiting_for_identification.erase(shared_from_this());
    main();
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
    connection->write(message.encodeMessage(), [this] () {onWriteMessage();});
}

void Session::onWriteMessage()
{
    recentMsgWrite.pop();
    if(!recentMsgWrite.empty())
    {
        writeMessage(recentMsgWrite.front());
    }
}

void Session::onError(SocketConnectionError error)
{
    logger->log(error.what());
    chat->leave(shared_from_this());
}

std::shared_ptr <Connection> Session::getConnection()
{
    return connection;
}