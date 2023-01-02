#include "Server.h"
#include "BasicAuth.h"
#include "BasicChat.h"
#include "LoggerComposite.h"
#include "StdOstreamLogger.h"
#include "FileLogger.h"

LoggerComposite global_logger_composite("",{});

void loggerSetup()
{
    Logger* cout_logger = new StdOstreamLogger(&std::cout);
    Logger* file_logger = new FileLogger(LOG_FILE_SERVER);

    global_logger_composite.addLogger(cout_logger);
    global_logger_composite.addLogger(file_logger);
}

int main(int argc, char* argv[])
{
    loggerSetup();
    boost::asio::io_context iocontext;

    std::shared_ptr <Logger> auth_logger_composite(new LoggerComposite("Server:\t", { &global_logger_composite }));
    std::shared_ptr <Logger> chat_logger_composite(new LoggerComposite("Chat:\t", { &global_logger_composite }));
    std::shared_ptr <Logger> server_logger_composite(new LoggerComposite("Server:\t", { &global_logger_composite }));

    BasicAuth auth(auth_logger_composite);
    BasicChat chat(&auth, chat_logger_composite);
    
    int port;
    
    if(argc == 1) port = 12345;
    else
    try
    {
        port = std::stoi(argv[1]);
    }
    catch(std::invalid_argument)
    {
        std::cout << "Bad port!\n";
        return 1;
    }

    Server server(iocontext, port, &chat, server_logger_composite);
    
    iocontext.run();

    return 0;
}