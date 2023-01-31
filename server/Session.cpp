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
    readMessage();
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
    MessageCreator f(w);
    auto message = f.get();

    MessageRelayVisitor relay(chat, auth, cp);
    message.acceptVisitor(relay);
}

void Session::identify(IdentifyCommandMessage id)
{
    ResponseMessage a_resp = chat->auth->authenticate(id);
    ResponseMessage s_resp;
    
    if(a_resp.ok())
    {
        s_resp = chat->auth->permitConnection(shared_from_this());
    }

    postMessage(a_resp);
    postMessage(s_resp);

    if(a_resp.ok() && s_resp.ok())
        onWriteIdentification();
    else
        onError(222);
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