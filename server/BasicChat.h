//FINISHED

#pragma once
#include "Chat.h"

#include "../defines.h"


class BasicChat : public Chat
{
public:
	BasicChat(Auth* _auth, std::vector<Logger*> _loggers);
	virtual void messageIncoming(ChatMessage message);
	virtual void join(boost::shared_ptr <Connection> connection);
	virtual void leave(boost::shared_ptr <Connection> connection);
	std::set <boost::shared_ptr <Connection>> getUserConnections(std::string username);
	std::set <boost::shared_ptr <Connection>> getIpConnections(std::string ip);
	virtual void log(std::string str);
	//	Auth* auth;
protected:
	
};