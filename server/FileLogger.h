#include "Logger.h"

class FileLogger : public Logger
{
public:
	FileLogger(std::string file);
	void log(std::string message);
private:
	std::string filename;
	std::ofstream stream;
};