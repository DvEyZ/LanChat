//FINISHED

#pragma once

#include "Connection.h"

class Connection;
class Chat;

class Server
{
public:
    Server(boost::asio::io_context& ioc, int port, Chat* _chat);
    ~Server();
    std::set <boost::shared_ptr <Connection>> awaiting_for_identification;
private:
    void start();
    void createConnection(boost::shared_ptr <Connection> connection, const boost::system::error_code& error);
    Chat* chat;
    boost::asio::io_context& _io_context;
    boost::asio::ip::tcp::acceptor _acceptor;
};