#include "Server.h"

int main(int argc, char* argv[])
{
    io_context iocontext;
    Server server = Server(iocontext, 12345);
    
    iocontext.run();
}