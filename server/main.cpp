#include "Server.h"
#include "BasicAuth.h"
#include "BasicChat.h"
#include "LoggerComposite.h"
#include "StdOstreamLogger.h"
#include "FileLogger.h"

LoggerComposite global_logger_composite;

void loggerSetup()
{
    auto cout_logger = new StdOstreamLogger(&std::cout);
    auto file_logger = new FileLogger(LOG_FILE_SERVER);

    global_logger_composite.addLogger(cout_logger);
    global_logger_composite.addLogger(file_logger);
}

int main(int argc, char* argv[])
{
    loggerSetup();
    boost::asio::io_context iocontext;
    BasicAuth auth;

    LoggerComposite chat_logger_composite("Chat:\t", {&global_logger_composite});
    LoggerComposite server_logger_composite("Server:\t", {&global_logger_composite});

    BasicChat chat(&auth, &chat_logger_composite);

    Server server(iocontext, 12345, &chat, &server_logger_composite);
    
    iocontext.run();

    return 0;
}