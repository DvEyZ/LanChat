#pragma once

#include <stdexcept>
#include <boost/system/error_code.hpp>

class ConnectionException : public std::exception
{
public:
	ConnectionException(const boost::system::error_code& _err);
	~ConnectionException();
	char* what();
private:
	boost::system::error_code err;
};