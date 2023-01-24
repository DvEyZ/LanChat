#pragma once

#include "../message/IdentifyCommandMessage.h"
#include "../message/ResponseMessage.h"
#include "../message/SendMessage.h"
#include "Chat.h"
#include "Logger.h"

#include <map>
#include <optional>

class Connection;
class Chat;
class Session;

class Auth
{
public:
	virtual ResponseMessage authenticate(IdentifyCommandMessage identification) = 0;
	virtual ResponseMessage permitConnection(std::shared_ptr <Session> session) = 0;
	virtual bool permitMessage(Message& message) = 0;
	void addToChat(Chat* chat);
protected:
	Chat* chat;
	std::shared_ptr <Logger> logger;
};