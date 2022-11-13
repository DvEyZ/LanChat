#include "Server.h"
#include "BasicAuth.h"
#include "BasicChat.h"
#include "StdOstreamLogger.h"

int main(int argc, char* argv[])
{
    boost::asio::io_context iocontext;
    BasicAuth auth;
    StdOstreamLogger cout_logger(&std::cout);

    BasicChat chat(&auth, {&cout_logger});

    Server server(iocontext, 12345, &chat);
    
    iocontext.run();
}