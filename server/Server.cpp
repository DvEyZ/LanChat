//FINISHED

#include "Server.h"

Server::Server(boost::asio::io_context& ioc, int port)
    :_io_context(ioc), _acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    start();
}

Server::~Server()
{}

void Server::start()
{
    boost::shared_ptr <Connection> connection = boost::shared_ptr<Connection>(new Connection(_io_context, chat));
    _acceptor.async_accept(connection->socket(), boost::bind(&Server::createConnection, this, connection, boost::asio::placeholders::error));
}

void Server::createConnection(boost::shared_ptr <Connection> connection, const boost::system::error_code& error)
{
    if(!error)
    {
        connection->run();
    }
    start();
}