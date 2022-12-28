#pragma once

#include <string>
#include <variant>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

class SocketConnectionError
{
public:
    enum Code
    {
        malformed_headers = 1
    };

    SocketConnectionError(boost::system::error_code code);
    SocketConnectionError(int code);
    std::variant <boost::system::error_code, int> getCode();
    std::string what();
private:
    std::variant <boost::system::error_code, int> code;
};