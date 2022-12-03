//FINISHED

#pragma once

#include "../ChatMessage.h"
#include "../defines.h"
#include "../IdentifyMessage.h"
#include "../IdentifyResponseMessage.h"
#include "Chat.h"
#include "Server.h"
#include <queue>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

class Chat;
class Server;

class Connection : public boost::enable_shared_from_this<Connection>
{
public:
    Connection(boost::asio::io_context& io_context_, Chat* chat, Server* server);
    ~Connection();
    boost::asio::ip::tcp::socket& socket();
    void run();             // main
    void postMessage(ChatMessage message);
    std::string getUser();
private:
    void identify();
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
    void onError(const boost::system::error_code& error);
    boost::asio::ip::tcp::socket sock;
    std::string user; 

    boost::system::error_code error_status;

    IdentifyMessage identifyMessageTemp;
    IdentifyResponseMessage identifyResponseMessageTemp;

    ChatMessage recentMsgRead;
    std::vector <char> recentReadBuffer;

    std::queue <ChatMessage> recentMsgWrite;

    Chat* chat;
    Server* server;
};