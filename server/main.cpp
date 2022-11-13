#include "Server.h"
#include "BasicAuth.h"
#include "BasicChat.h"

int main(int argc, char* argv[])
{
    boost::asio::io_context iocontext;
    BasicAuth auth;
    BasicChat chat(&auth);
    auth.addToChat(&chat);

    Server server(iocontext, 12345, &chat);
    
    iocontext.run();
}