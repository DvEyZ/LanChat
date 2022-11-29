//FINISHED

#include "Server.h"

Server::Server(boost::asio::io_context& ioc, int port, Chat* _chat)
    :_io_context(ioc), _acceptor(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), chat(_chat)
{
    awaiting_for_identification = std::set<boost::shared_ptr<Connection>>();
    chat->log("Server runs on host " + _acceptor.local_endpoint().address().to_string() + " on port " + std::to_string(port) + ".");
    start();
}

Server::~Server()
{}

void Server::start()
{
    boost::shared_ptr <Connection> connection = boost::shared_ptr<Connection>(new Connection(_io_context, chat));
    _acceptor.async_accept(connection->socket(), boost::bind(&Server::createConnection, this, connection, boost::asio::placeholders::error));
    chat->log("Server started - awaiting for connections.");
}

void Server::createConnection(boost::shared_ptr <Connection> connection, const boost::system::error_code& error)
{
    if(!error)
    {
        connection->run();
        chat->log("Connection from " + connection->socket().remote_endpoint().address().to_string() + " accepted.");
    }
    else
    {
        chat->log("Error while accepting connection - " + error.to_string());

    }
    start();
}