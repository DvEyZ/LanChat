#include <iostream>
#include <cstring>
#include <thread>
#include <boost/asio.hpp>
#include "App.h"

int main(int argc, char* argv[])
{
    std::vector <std::string> args;

    for(int i = 1; i < argc; i++)
    {
        args.push_back(argv[i]);    
    }
    
    App app(args);

    app.run();

    return 0;
}