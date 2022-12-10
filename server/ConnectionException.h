#pragma once

#include <stdexcept>
#include <boost/system/error_code.hpp>

class ConnectionException : public std::exception
{
public:
	ConnectionException(std::string _err);
	~ConnectionException();
	char* what();
private:
	std::string err;
};