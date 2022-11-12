//FINISHED

#pragma once
#include "Connection.h"
#include "Auth.h"
#include "defines.h"
#include <set>
#include <iostream>
#include <string>
#include <fstream>

class Chat
{
public:
	Chat();
	void log(std::string message);
	void messageIncoming(ChatMessage message);
	void join(boost::shared_ptr <Connection> connection);
	void leave(boost::shared_ptr <Connection> connection);
	std::set <boost::shared_ptr <Connection>> getUserConnections(std::string username);
	std::set <boost::shared_ptr <Connection>> getIpConnections(std::string ip);
	Auth auth;
private:
	std::set <boost::shared_ptr <Connection>> connected;
	std::fstream logstream;
};