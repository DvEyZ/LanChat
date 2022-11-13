#pragma once
#include <iostream>
#include <fstream>
#include <string>

class Logger
{
public:
	virtual void log(std::string message) = 0;
};