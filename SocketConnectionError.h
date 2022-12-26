#pragma once

#include <string>
#include <map>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

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