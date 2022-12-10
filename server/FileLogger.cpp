#include "FileLogger.h"

FileLogger::FileLogger(std::string file)
	:filename(file), stream(filename)
{
	stream.close();
}

void FileLogger::log(std::string message)
{
	stream.open(filename);
	stream << message << "\n";
	stream.close();
}