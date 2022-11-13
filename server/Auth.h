#pragma once

#include "../IdentifyMessage.h"
#include "../IdentifyResponseMessage.h"
#include "Chat.h"

#include <map>
#include <optional>

class Auth
{
public:
	virtual IdentifyResponseMessage::Status authenticate(IdentifyMessage identification) = 0;
	virtual IdentifyResponseMessage::Status permitConnection(boost::shared_ptr <Connection> connection) = 0;
	virtual bool permitMessage(ChatMessage message) = 0;
	void addToChat(Chat* chat);
};