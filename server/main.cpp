#include "Server.h"

int main(int argc, char* argv[])
{
    boost::asio::io_context iocontext;
    Server server(iocontext, 12345);
    
    iocontext.run();
}