//FINISHED

#pragma once

#include "../SocketConnection.h"
#include "../ChatMessage.h"
#include "../defines.h"
#include "../IdentifyMessage.h"
#include "../IdentifyResponseMessage.h"
#include "Chat.h"
#include "Server.h"
#include <queue>
#include <functional>
#include <boost/enable_shared_from_this.hpp>

class Chat;
class Server;
class SocketConnectionError;

class Session : public std::enable_shared_from_this<Session>
{
public:
    static std::set <std::shared_ptr <Session>> awaiting_for_identification;

    Session(std::shared_ptr<Connection> connection, Chat* chat, std::shared_ptr <Logger>  logger);
    ~Session();
    void run();             // main
    void postMessage(ChatMessage message);
    
    std::string getUser();
    std::shared_ptr <Connection> getConnection();
private:
    void identify(IdentifyMessage id);
    void main();

    void readIdentification();
    void onReadIdentification(std::vector <char> message);      // Connection::onRead callback
    void readMessage();
    void onReadMessage(std::vector <char> message);             // Connection::onRead callback

    void writeIdentification(IdentifyResponseMessage resp);
    void onWriteIdentification(IdentifyResponseMessage::Status status);     // Connection::onWrite callback
    
    void writeMessage(ChatMessage message);
    void onWriteMessage();
    
    void identificationFailure(IdentifyResponseMessage::Status status);
    
    void onError(SocketConnectionError error);
    
    std::queue <ChatMessage> recentMsgWrite;

    std::string user; 
    std::shared_ptr <Connection> connection;
    Chat* chat;

    std::shared_ptr <Logger>  logger;
};