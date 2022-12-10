#pragma once

#include "Logger.h"
#include "../defines.h"
#include <functional>
#include <vector>

class Connection
{
public:
	virtual void read(std::function<void(std::vector <char>)> callback) = 0;
	virtual void write(std::vector <char> text, std::function<void ()> callback) = 0;
	virtual std::string getRemoteIp() = 0;
private:
};