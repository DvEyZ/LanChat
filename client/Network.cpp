#include "Network.h"

Network::Network(App* _app)
    :app(_app), context(new boost::asio::io_context())
{
}

void Network::run()
{
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> guard(context->get_executor());
    context->run();
}

void Network::connect(std::string addr, std::function <void(std::shared_ptr <SocketConnection>)> callback) // addr: address:[port/default 12345]
{
    connection = std::shared_ptr <SocketConnection> (new SocketConnection(*context));
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
    return std::pair<std::string, std::string> (hname, port);
}

void Network::resolveHostname(std::pair <std::string, std::string> host)
{
    boost::asio::ip::tcp::resolver resolver(*context);
    
    try 
    {
        auto res = resolver.resolve(host.first, host.second);
        connectToEndpoint(res);
    }
    catch(std::exception& e)
    {
        app->error(e.what());
    }
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