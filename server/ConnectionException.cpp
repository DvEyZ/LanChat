#include "ConnectionException.h"

ConnectionException::ConnectionException(const boost::system::error_code& _err)
	:err(_err)
{}

ConnectionException::~ConnectionException()
{}

char* ConnectionException::what()
{
	return err.message().data();
}