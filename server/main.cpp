#include "Server.h"
#include "BasicAuth.h"
#include "BasicChat.h"
#include "StdOstreamLogger.h"
#include "FileLogger.h"

int main(int argc, char* argv[])
{
    boost::asio::io_context iocontext;
    BasicAuth auth;
    StdOstreamLogger cout_logger(&std::cout);
    FileLogger file_logger(LOG_FILE_SERVER);

    BasicChat chat(&auth, {&cout_logger, &file_logger});

    Server server(iocontext, 12345, &chat);
    
    iocontext.run();
}