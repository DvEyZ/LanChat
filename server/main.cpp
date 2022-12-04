#include "Server.h"
#include "BasicAuth.h"
#include "BasicChat.h"
#include "LoggerProxy.h"
#include "StdOstreamLogger.h"
#include "FileLogger.h"

LoggerProxy global_logger_proxy;

void loggerSetup()
{
    auto cout_logger = new StdOstreamLogger(&std::cout);
    auto file_logger = new FileLogger(LOG_FILE_SERVER);

    global_logger_proxy.addLogger(cout_logger);
    global_logger_proxy.addLogger(file_logger);
}

int main(int argc, char* argv[])
{
    loggerSetup();
    boost::asio::io_context iocontext;
    BasicAuth auth;

    LoggerProxy chat_logger_proxy("Chat:\t", {&global_logger_proxy});
    LoggerProxy server_logger_proxy("Server:\t", {&global_logger_proxy});

    BasicChat chat(&auth, &chat_logger_proxy);

    Server server(iocontext, 12345, &chat, &server_logger_proxy);
    
    iocontext.run();

    return 0;
}