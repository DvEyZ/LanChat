//FINISHED

#pragma once
#include "Chat.h"

#include "../defines.h"


class BasicChat : public Chat
{
public:
	BasicChat(Auth* _auth);
	void log(std::string message);
	void messageIncoming(ChatMessage message);
	void join(boost::shared_ptr <Connection> connection);
	void leave(boost::shared_ptr <Connection> connection);
	std::set <boost::shared_ptr <Connection>> getUserConnections(std::string username);
	std::set <boost::shared_ptr <Connection>> getIpConnections(std::string ip);
	//	Auth* auth;
private:
	std::set <boost::shared_ptr <Connection>> connected;
	std::fstream logstream;
};