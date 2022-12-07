#pragma once

#include "Logger.h"
#include <vector>

class LoggerComposite : public Logger
{
public:
	LoggerComposite(std::string _module_name, std::vector <Logger*> _loggers);
	void log(std::string message);
	void addLogger(Logger* logger);
private:
	std::string module_name;
	std::vector <Logger*> loggers;
};