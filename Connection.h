#pragma once

#include "SocketConnectionError.h"
#include "defines.h"
#include <functional>
#include <vector>

class Connection
{
public:
	virtual ~Connection() {};
	virtual void read(std::function<void(std::vector <char>)> callback) = 0;
	virtual void write(std::vector <char> text, std::function<void ()> callback) = 0;
	virtual void setErrorCallback(std::function <void(SocketConnectionError)> callback) = 0;
	virtual std::string getRemoteIp() = 0;
	virtual void close() = 0;
};