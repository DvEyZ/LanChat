//FINISHEDD

#include "Session.h"

std::set <std::shared_ptr <Session>> Session::awaiting_for_identification;

Session::Session(std::shared_ptr<Connection> _connection, Chat* _chat, std::shared_ptr <Logger>  _logger)
    :connection(_connection), chat(_chat), logger(_logger)
{
    connection->setErrorCallback(
        [this] (ConnectionError err) mutable
        {
            onError(err);
        }
    );
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
    connection->read([this] (MessageWrapper w) { onReadMessage(w);});
}

void Session::onReadMessage(MessageWrapper w)
{
    SendMessage m;
    if(m.decode(w.getData()))
    {
        chat->messageIncoming(m);
    }
    else
    {
        ResponseMessage m({"Malformed message."}, ResponseMessage::MALFORMED_MESSAGE);
        postMessage(m);
    }
    main();
}

void Session::readIdentification()
{
    connection->read([this] (MessageWrapper w) {onReadIdentification(w);});
}

void Session::onReadIdentification(MessageWrapper message)
{
    IdentifyMessage id;
    if(id.decode(message.getData()))
    {
        identify(id);
    }
    else
    {
        ResponseMessage m({"Malformed message."}, ResponseMessage::MALFORMED_MESSAGE);
        writeMessage(m, [this] () { onError(ConnectionError(ResponseMessage::MALFORMED_MESSAGE)); });
    }
}

void Session::identify(IdentifyMessage id)
{
    ResponseMessage resp;

    // REORGANIZACJA

    writeMessage(resp, 
        [this, resp] () mutable
        {
            if(resp.ok())
                onWriteIdentification();
            else
                onError(resp.getStatus());
        } 
    );
}

void Session::onWriteIdentification()
{
    chat->join(shared_from_this());
    Session::awaiting_for_identification.erase(shared_from_this());
    main();
}

void Session::postMessage(Message& message)
{
    bool idle = recentMsgWrite.empty();
    recentMsgWrite.push(message);
    if(idle)
    {
        writeMessage(recentMsgWrite.front(), [this] () { onWriteMessage(); });
    }
}

void Session::writeMessage(Message& message, std::function <void()> callback)
{
    MessageWrapper wrapper(message);
    connection->write(wrapper, callback);
}

void Session::onWriteMessage()
{
    recentMsgWrite.pop();
    if(!recentMsgWrite.empty())
    {
        writeMessage(recentMsgWrite.front(), [this] () { onWriteMessage(); });
    }
}

void Session::onError(ConnectionError error)
{
    logger->log(error.what());
    chat->leave(shared_from_this());
}

std::shared_ptr <Connection> Session::getConnection()
{
    return connection;
}