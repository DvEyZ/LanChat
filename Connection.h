#pragma once

#include "ConnectionError.h"
#include "MessageWrapper.h"
#include "defines.h"
#include <functional>
#include <vector>

class Connection
{
public:
	virtual ~Connection() {};
	virtual void read(std::function<void(MessageWrapper)> callback) = 0;
	virtual void write(MessageWrapper text, std::function<void ()> callback) = 0;
	virtual void setErrorCallback(std::function <void(ConnectionError)> callback) = 0;
	virtual std::string getRemoteIp() = 0;
	virtual void close() = 0;
};