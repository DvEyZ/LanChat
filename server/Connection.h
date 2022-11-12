//FINISHED

#pragma once

#include "../ChatMessage.h"
#include "../IdentifyMessage.h"
#include "../IdentifyResponseMessage.h"
#include "Chat.h"
#include "Server.h"
#include <queue>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

class Connection : public boost::enable_shared_from_this<Connection>
{
public:
    Connection(boost::asio::io_context io_context_, Chat* chat);
    ~Connection();
    boost::asio::ip::tcp::socket& socket();
    void run();             // main
    void postMessage(ChatMessage message);
    std::string user;       // User of this session, by default 
private:
    bool identify();
    void identificationFailure(IdentifyResponseMessage::Status status);
    void readIdentificationHeader();
    void onReadIdentificationHeader(const boost::system::error_code& error, std::size_t bytes_transferred);
    void readIdentificationBody();
    void onReadIdentificationBody(const boost::system::error_code& error, std::size_t bytes_transferred);
    void writeIdentificationResponse();
    void onWriteIdentificationResponse(const boost::system::error_code& error, std::size_t bytes_transferred);
    void readHeader();
    void readBody();
    void write();
    void onReadHeader(const boost::system::error_code& error, std::size_t bytes_transferred);
    void onReadBody(const boost::system::error_code& error, std::size_t bytes_transferred);
    void onWrite(const boost::system::error_code& error, std::size_t bytes_transferred);
    void onError();
    boost::asio::ip::tcp::socket sock;
    boost::system::error_code error_status;
    ChatMessage recentMsgRead;
    char* recentMsgHeaderBuffer;
    char* recentMsgBodyBuffer;
    std::queue <ChatMessage> recentMsgWrite;

    IdentifyMessage* identifyMessageTemp;
    IdentifyResponseMessage* identifyResponseMessageTemp;

    Chat* chat;
    Server* server;
};