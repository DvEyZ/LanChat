#pragma once

#include <string>
#include <variant>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

class ConnectionError
{
public:
    enum Code
    {
        malformed_headers = 1
    };

    ConnectionError(boost::system::error_code code);
    ConnectionError(int code);
    std::variant <boost::system::error_code, int> getCode();
    std::string what();
private:
    std::variant <boost::system::error_code, int> code;
};