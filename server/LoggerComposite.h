#pragma once

#include "Logger.h"
#include <vector>
#include <memory>

class LoggerComposite : public Logger
{
public:
	LoggerComposite(std::string _module_name, std::vector < std::shared_ptr <Logger> > _loggers);
	void log(std::string message);
	void addLogger(std::shared_ptr <Logger> logger);
private:
	std::string module_name;
	std::vector < std::shared_ptr <Logger> > loggers;
};