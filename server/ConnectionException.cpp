#include "ConnectionException.h"

ConnectionException::ConnectionException(std::string _err)
	:err(_err)
{}

ConnectionException::~ConnectionException()
{}

char* ConnectionException::what()
{
	return err.data();
}