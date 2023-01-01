#include "LoggerComposite.h"

LoggerComposite::LoggerComposite(std::string _module_name, std::vector <Logger*> _loggers)
	:module_name(_module_name), loggers(_loggers)
{
}

void LoggerComposite::log(std::string message)
{
	for(auto i : loggers)
	{
		i->log(module_name + message);
	}
}

void LoggerComposite::addLogger(Logger* logger)
{
	loggers.push_back(logger);
}