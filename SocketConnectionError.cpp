#include "SocketConnectionError.h"

SocketConnectionError::SocketConnectionError(Type _type, int _code)
    :type(_type), code(_code)
{}

SocketConnectionError::Type SocketConnectionError::getType()
{
    return type;
}

int SocketConnectionError::getCode()
{
    return code;
}

std::string SocketConnectionError::what()
{
    if(type == SocketConnectionError::Type::asio_error)
    {
        return boost::system::error_code(code, boost::asio::error::get_misc_category()).message();
    }
    else
    {
        return std::string("Error ") + std::to_string(code);
    }
}