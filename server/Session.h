//FINISHED

#pragma once

#include "Connection.h"
#include "../ChatMessage.h"
#include "../defines.h"
#include "../IdentifyMessage.h"
#include "../IdentifyResponseMessage.h"
#include "Chat.h"
#include "Server.h"
#include <queue>
#include <functional>

class Chat;
class Server;

class Session : public boost::enable_shared_from_this<Session>
{
public:
    static std::set <boost::shared_ptr <Session>> awaiting_for_identification;

    Session(boost::shared_ptr<Connection> connection, Chat* chat);
    ~Session();
    void run();             // main
    void postMessage(ChatMessage message);
    
    std::string getUser();
    boost::shared_ptr <Connection> getConnection();
private:
    void identify(IdentifyMessage id);
    void main();

    void readIdentification();
    void onReadIdentification(std::vector <char> message);      // Connection::onRead callback
    void readMessage();
    void onReadMessage(std::vector <char> message);             // Connection::onRead callback

    void writeIdentification(IdentifyResponseMessage resp);
    void onWriteIdentification(IdentifyResponseMessage::Status status);                               // Connection::onWrite callback
    void writeMessage(ChatMessage message);
    void onWriteMessage();
    
    void identificationFailure(IdentifyResponseMessage::Status status);
    void onError(const boost::system::error_code& error);
    
    std::queue <ChatMessage> recentMsgWrite;

    std::string user; 
    boost::shared_ptr <Connection> connection;
    Chat* chat;
};