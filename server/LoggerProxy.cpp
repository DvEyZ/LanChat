#include "LoggerProxy.h"

LoggerProxy::LoggerProxy(std::string _module_name, std::vector <Logger*> _loggers)
	:module_name(_module_name), loggers(_loggers)
{
}

void LoggerProxy::log(std::string message)
{
	message = "[ " + module_name + " ]: " + message;
	for(auto i : loggers)
	{
		i->log(message);
	}
}