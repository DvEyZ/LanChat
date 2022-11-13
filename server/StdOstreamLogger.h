#include "Logger.h"

class StdOstreamLogger : public Logger
{
public:
	StdOstreamLogger(std::ostream* _stream);
	virtual void log(std::string message);
private:
	std::ostream* stream;
};