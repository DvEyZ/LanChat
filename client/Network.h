#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <functional>
#include "../SocketConnection.h"
#include "App.h"

class App;

class Network
{
public:
    Network(App* app);
    void connect(std::string addr, std::function <void(std::shared_ptr <SocketConnection>)> callback);
    void run();
    void stop();
private:
    std::pair <std::string, std::string> splitHostname(std::string hname);
    void resolveHostname(std::pair <std::string, std::string> host);
    void connectToEndpoint(boost::asio::ip::tcp::resolver::results_type endpoints);
    void onConnect();

    std::shared_ptr <SocketConnection> connection;
    std::function <void(std::shared_ptr <SocketConnection>)> connect_callback;

    std::shared_ptr <boost::asio::io_context> context;
    std::unique_ptr <boost::asio::executor_work_guard <boost::asio::io_context::executor_type>> guard;
    App* app;
};