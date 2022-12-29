#include <iostream>
#include <cstring>
#include <thread>
#include <boost/asio.hpp>
#include "App.h"

int main(int argc, char* argv[])
{
    boost::asio::io_context io_context;

    std::vector <std::string> args;

    for(int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);    
    }
    
    Network network(io_context);
    App app(network, args);

    app.run();

    return 0;
}