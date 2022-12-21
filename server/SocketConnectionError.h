#pragma once

#include <string>

class SocketConnectionError
{
public:
    

    enum Type
    {
        asio_error,
        program_error
    };

    enum Code
    {
        malformed_headers = 1
    };
    
    SocketConnectionError(Type type, int code);
    Type getType();
    int getCode();
    std::string what();
private:
    Type type;
    int code;
};