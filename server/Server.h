//FINISHED

#pragma once

#include "Session.h"
#include "LoggerComposite.h"
#include <boost/bind.hpp>

class Connection;
class Chat;
extern LoggerComposite global_logger_composite;

class Server
{
public:
    Server(boost::asio::io_context& ioc, int port, Chat* _chat, Logger* _logger);
    ~Server();
private:
    void start();
    void createConnection(std::shared_ptr <Connection> connection, const boost::system::error_code& error);
    Chat* chat;
    boost::asio::io_context& _io_context;
    boost::asio::ip::tcp::acceptor _acceptor;
    Logger* logger;
};