#include "StdOstreamLogger.h"

StdOstreamLogger::StdOstreamLogger(std::ostream* _stream)
	:stream(_stream)
{}

void StdOstreamLogger::log(std::string message)
{
	*stream << message << "\n";
}