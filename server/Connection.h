#pragma once

#include "Logger.h"
#include "SocketConnectionError.h"
#include "../defines.h"
#include <functional>
#include <vector>

class Connection
{
public:
	virtual void read(std::function<void(std::vector <char>)> callback) = 0;
	virtual void write(std::vector <char> text, std::function<void ()> callback) = 0;
	void setErrorCallback(std::function <void(SocketConnectionError)> callback);
	virtual std::string getRemoteIp() = 0;
protected:
	std::function <void(SocketConnectionError)> error_callback;
};