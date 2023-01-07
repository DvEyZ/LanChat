//FINISHED

#pragma once

#include "../SocketConnection.h"
#include "../defines.h"
#include "Chat.h"
#include "Server.h"
#include "../message/IdentifyMessage.h"
#include "../message/SendMessage.h"
#include "../message/ResponseMessage.h"
#include <queue>
#include <functional>
#include <boost/enable_shared_from_this.hpp>

class Chat;
class Server;
class ConnectionError;

class Session : public std::enable_shared_from_this<Session>
{
public:
    static std::set <std::shared_ptr <Session>> awaiting_for_identification;

    Session(std::shared_ptr<Connection> connection, Chat* chat, std::shared_ptr <Logger>  logger);
    ~Session();
    void run();             // main
    void postMessage(Message& message);
    
    std::string getUser();
    std::shared_ptr <Connection> getConnection();
private:
    void identify(IdentifyMessage id);
    void main();

    void readIdentification();
    void onReadIdentification(MessageWrapper w);      // Connection::onRead callback
    void readMessage();
    void onReadMessage(MessageWrapper w);             // Connection::onRead callback
    
    void writeMessage(Message& message, std::function <void()> callback);
    void onWriteMessage();
    void onWriteIdentification();

    void onError(ConnectionError error);
    
    std::queue <Message&> recentMsgWrite;

    std::string user; 
    std::shared_ptr <Connection> connection;
    Chat* chat;

    std::shared_ptr <Logger>  logger;
};