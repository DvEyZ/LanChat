#pragma once

#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include "../../server/Connection.h"
#include "../../server/ConnectionException.h"

class TestConnection : public Connection
{
public:
	TestConnection(std::stringstream* _sstream);
	void read(std::function<void(std::vector <char>)> callback);
	void write(std::vector <char> text, std::function<void ()> callback);
	std::string getRemoteIp();
private:
	std::stringstream* sstream;
	std::string readHeader();
	std::string readBody();
};