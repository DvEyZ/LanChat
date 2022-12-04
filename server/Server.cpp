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
    boost::shared_ptr<Connection> connection(new Connection(_io_context));
    _acceptor.async_accept(connection->getSocket(), boost::bind(&Server::createConnection, this, connection, boost::asio::placeholders::error));
}

void Server::createConnection(boost::shared_ptr <Connection> connection, const boost::system::error_code& error)
{
    if(!error)
    {
        boost::shared_ptr <Session> session(new Session(connection, chat));
        session->run();
        logger->log("Connection from " + connection->getSocket().remote_endpoint().address().to_string() + " accepted.");
    }
    else
    {
        logger->log("Error while accepting connection - " + error.to_string());

    }
    start();
}