#include "Network.h"

Network::Network(boost::asio::io_context& _io_context)
    :context(_io_context)
{
}

void Network::run()
{
    context.run();
}

void Network::connect(std::string addr, std::function <void(std::shared_ptr <SocketConnection>)> callback) // addr: address:[port/default 12345]
{
    connection = std::shared_ptr <SocketConnection> (new SocketConnection(context));
    connect_callback = callback;
    resolveHostname(splitHostname(addr));
}

std::pair <std::string, std::string> Network::splitHostname(std::string host)
{
    size_t delim = host.find(':');

    std::string hname;
    std::string port;
    if(delim != std::string::npos)
    {
        hname = host.substr(0, delim);
        port = host.substr(delim+1);
    }
    else
    {
        hname = host;
        port = "12345";
    }
}

void Network::resolveHostname(std::pair <std::string, std::string> host)
{
    boost::asio::ip::tcp::resolver resolver(context);

    resolver.async_resolve(host.first, host.second, 
        [this] (boost::system::error_code error, boost::asio::ip::tcp::resolver::results_type endpoints)
        {
            if(!error)
            {
                connectToEndpoint(endpoints);
            }
            else
            {
                app->error("Error while resolving hostname: " + error.message());
            }
        }
    );
}

void Network::connectToEndpoint(boost::asio::ip::tcp::resolver::results_type endpoints)
{
    connection->getSocket().async_connect(endpoints->endpoint(), 
        [this] (boost::system::error_code error) 
        {
            if(!error)
            {
                onConnect();
            }
            else
            {
                app->error("Error while connecting: " + error.message());
            }
        }
    );
}

void Network::onConnect()
{
    connect_callback(connection);
}