//FINISHED

#pragma once

#include "Connection.h"

class Server
{
public:
    Server(boost::asio::io_context& ioc, int port = 6789);
    std::set <boost::asio::shared_ptr <Connection>> awaiting_for_identification;
private:
    ~Server();
    void start();
    void createConnection(shared_ptr <Connection> connection, const asio::error_code& error);
    Chat* chat;
    boost::asio::io_context _io_context;
    boost::asio::ip::tcp::acceptor _acceptor;
};