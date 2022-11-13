#pragma once

#include <set>
#include <iostream>
#include <string>
#include <fstream>
#include "Connection.h"
#include "Auth.h"
#include "Logger.h"

class Chat
{
public:
	virtual void messageIncoming(ChatMessage message) = 0;
	virtual void join(boost::shared_ptr <Connection> connection) = 0;
	virtual void leave(boost::shared_ptr <Connection> connection) = 0;
	virtual std::set <boost::shared_ptr <Connection>> getUserConnections(std::string username) = 0;
	virtual std::set <boost::shared_ptr <Connection>> getIpConnections(std::string ip) = 0;
	Auth* auth;
	std::vector <Logger*> loggers;
};