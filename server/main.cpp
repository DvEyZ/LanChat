#include "Server.h"
#include "BasicAuth.h"
#include "BasicChat.h"
#include "LoggerProxy.h"
#include "StdOstreamLogger.h"
#include "FileLogger.h"

int main(int argc, char* argv[])
{
    boost::asio::io_context iocontext;
    BasicAuth auth;
    StdOstreamLogger cout_logger(&std::cout);
    FileLogger file_logger(LOG_FILE_SERVER);

    LoggerProxy chat_logger_proxy({&cout_logger, &file_logger});
    LoggerProxy server_logger_proxy({&cout_logger, &file_logger});

    BasicChat chat(auth, &chat_logger_proxy);
    auth.addToChat(&chat);

    Server server(iocontext, 12345, &chat, &server_logger_proxy);
    
    iocontext.run();

    return 0;
}