//FINISHED

#pragma once
#include "Chat.h"

#include "../defines.h"


class BasicChat : public Chat
{
public:
	BasicChat(Auth* _auth, std::shared_ptr <Logger> _logger);
	virtual void messageIncoming(Addressed& message);
	virtual void join(std::shared_ptr <Session> session);
	virtual void leave(std::shared_ptr <Session> session);
	std::set <std::shared_ptr <Session>> getUserConnections(std::string username);
	std::set <std::shared_ptr <Session>> getIpConnections(std::string ip);
	//	Auth* auth;
protected:
	
};