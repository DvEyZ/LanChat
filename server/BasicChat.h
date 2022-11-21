//FINISHED

#pragma once
#include "Chat.h"

#include "../defines.h"


class BasicChat : public Chat
{
public:
	BasicChat(Auth* _auth, std::vector<Logger*> _loggers);
	void messageIncoming(ChatMessage message);
	void join(boost::shared_ptr <Connection> connection);
	void leave(boost::shared_ptr <Connection> connection);
	std::set <boost::shared_ptr <Connection>> getUserConnections(std::string username);
	std::set <boost::shared_ptr <Connection>> getIpConnections(std::string ip);
	void log(std::string str);
	//	Auth* auth;
private:
	std::set <boost::shared_ptr <Connection>> connected;
};