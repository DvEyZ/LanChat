#pragma once

#include <gtest/gtest.h>
#include <boost/asio.hpp>
#include "../../server/Connection.h"
#include "DummyNetwork.h"

class TestConnection : public Connection
{
public:
	TestConnection(DummyNetwork* network);
	void read(std::function<void(std::vector <char>)> callback);
	void write(std::vector <char> text, std::function<void ()> callback);
	std::string getRemoteIp();
private:
	DummyNetwork* network;
	std::string readHeader();
	std::string readBody();
};