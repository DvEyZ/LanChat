#include "SocketConnectionError.h"

SocketConnectionError::SocketConnectionError(int _code)
    :code(_code)
{}

SocketConnectionError::SocketConnectionError(boost::system::error_code _code)
    :code(_code)
{}

std::variant <boost::system::error_code, int> SocketConnectionError::getCode()
{
    return code;
}

std::string SocketConnectionError::what()
{
    if(code.index() == 0)
    {
        std::get<boost::system::error_code>(code).message();

    }
    else
    {
        return std::string("Error ") + std::to_string(std::get <int> (code));
    }
}