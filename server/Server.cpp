//FINISHED

#include "Server.h"

Server::Server(boost::asio::io_context& ioc, int port, Chat* _chat, Logger* _logger)
    :_io_context(ioc), _acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), chat(_chat), logger(_logger)
{
    logger->log("Server runs on host " + _acceptor.local_endpoint().address().to_string() + " on port " + std::to_string(port) + ".");
    start();
}

Server::~Server()
{}

void Server::start()
{
    std::shared_ptr<Connection> connection(new SocketConnection(_io_context));
    _acceptor.async_accept(std::dynamic_pointer_cast<SocketConnection>(connection)->getSocket(), boost::bind(&Server::createConnection, this, connection, boost::asio::placeholders::error));
}

void Server::createConnection(std::shared_ptr <Connection> connection, const boost::system::error_code& error)
{
    if(!error)
    {
        boost::shared_ptr <Session> session(new Session(connection, chat, new LoggerProxy("Session - " + connection->getRemoteIp() + ":", {&global_logger_proxy})));
        session->run();
        logger->log("Connection from " + connection->getRemoteIp() + " accepted.");
    }
    else
    {
        logger->log("Error while accepting connection - " + error.message());

    }
    start();
}