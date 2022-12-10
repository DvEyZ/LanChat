#pragma once

#include <set>
#include <iostream>
#include <string>
#include <fstream>
#include "Session.h"
#include "Auth.h"
#include "Logger.h"

class Connection;
class Auth;
class Session;

class Chat
{
public:
	virtual void messageIncoming(ChatMessage message) = 0;
	virtual void join(std::shared_ptr <Session> session) = 0;
	virtual void leave(std::shared_ptr <Session> session) = 0;
	virtual std::set <std::shared_ptr <Session>> getUserConnections(std::string username) = 0;
	virtual std::set <std::shared_ptr <Session>> getIpConnections(std::string ip) = 0;
	Auth* auth;
protected:
	std::shared_ptr <Logger> logger;
	std::set <std::shared_ptr <Session>> connected;
};