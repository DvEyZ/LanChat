#include "ConnectionError.h"

ConnectionError::ConnectionError(int _code)
    :code(_code)
{}

ConnectionError::ConnectionError(boost::system::error_code _code)
    :code(_code)
{}

std::variant <boost::system::error_code, int> ConnectionError::getCode()
{
    return code;
}

std::string ConnectionError::what()
{
    if(code.index() == 0)
    {
        return std::get<boost::system::error_code>(code).message();
    }
    else
    {
        return std::string("Error ") + std::to_string(std::get <int> (code));
    }
}