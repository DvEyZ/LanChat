//FINISHED

#pragma once
#include "Chat.h"

#include "../defines.h"


class BasicChat : public Chat
{
public:
	BasicChat(Auth* _auth, Logger* _logger);
	virtual void messageIncoming(ChatMessage message);
	virtual void join(boost::shared_ptr <Session> session);
	virtual void leave(boost::shared_ptr <Session> session);
	std::set <boost::shared_ptr <Session>> getUserConnections(std::string username);
	std::set <boost::shared_ptr <Session>> getIpConnections(std::string ip);
	//	Auth* auth;
protected:
	
};