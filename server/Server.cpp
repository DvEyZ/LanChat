//FINISHED

#include "Server.h"

Server::Server(boost::asio::io_context& ioc, int port, Chat* _chat, std::shared_ptr <Logger> _logger)
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
    _acceptor.async_accept(
        std::dynamic_pointer_cast<SocketConnection>(connection)->getSocket(),
        [this, connection](const boost::system::error_code& error)
        {
            createConnection(connection, error);
        }
    );
}

void Server::createConnection(std::shared_ptr <Connection> connection, const boost::system::error_code& error)
{
    if(!error)
    {
        std::shared_ptr <Logger> new_logger_composite(new LoggerComposite("Session - " + connection->getRemoteIp() + ": ", { &global_logger_composite}));
        std::shared_ptr <Session> session(new Session(connection, chat, new_logger_composite));
        session->run();
        logger->log("Connection from " + connection->getRemoteIp() + " accepted.");
    }
    else
    {
        logger->log("Error while accepting connection - " + error.message());

    }
    start();
}