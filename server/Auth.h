#pragma once

#include "../IdentifyMessage.h"
#include "../IdentifyResponseMessage.h"
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
	virtual IdentifyResponseMessage::Status authenticate(IdentifyMessage identification) = 0;
	virtual IdentifyResponseMessage::Status permitConnection(std::shared_ptr <Session> session) = 0;
	virtual bool permitMessage(ChatMessage message) = 0;
	void addToChat(Chat* chat);
protected:
	Chat* chat;
	std::shared_ptr <Logger> logger;
};