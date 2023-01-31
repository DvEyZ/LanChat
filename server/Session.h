//FINISHED

#pragma once

#include "../SocketConnection.h"
#include "../defines.h"
#include "Chat.h"
#include "Auth.h"
#include "../MessageCreator.h"
#include "../message/IdentifyCommandMessage.h"
#include "../message/SendMessage.h"
#include "../message/ResponseMessage.h"
#include <queue>
#include <functional>

class Chat;
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
    void identify(IdentifyCommandMessage id);
    void main();

    void readMessage();
    void onReadMessage(MessageWrapper w);             // Connection::onRead callback
    
    void writeMessage(Message& message, std::function <void()> callback);
    void onWriteMessage();
    void onWriteIdentification();

    void onError(ConnectionError error);
    
    std::queue <Message> recentMsgWrite;

    std::string user; 
    std::shared_ptr <Connection> connection;
    
    Auth* auth;
    Chat* chat;

    std::shared_ptr <Logger>  logger;
};