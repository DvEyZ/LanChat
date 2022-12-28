#pragma once

#include <boost/asio.hpp>
#include <vector>
#include <string>
#include "CLI.h"
#include "Session.h"

class CLI;

class App
{
public:
    App(boost::asio::io_context& _io_context, std::vector <std::string> argv);
    ~App();

    void run();
private:
    CLI cli;
    Session session;
};