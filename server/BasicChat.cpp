//FINISHED

#include "BasicChat.h"

BasicChat::BasicChat(Auth* _auth, std::shared_ptr <Logger> _logger)
{
	auth = _auth;
	logger = _logger;
	auth->addToChat(this);
}

void BasicChat::join(std::shared_ptr <Session> session)
{
	connected.insert(session);
	logger->log(session->getUser() + " joined the chat.");
}

void BasicChat::leave(std::shared_ptr <Session> session)
{
	logger->log(session->getUser() + " left the chat.");
	connected.erase(session);
}

void BasicChat::messageIncoming(ChatMessage message)
{
	// system messages don't need permission from Auth
	if(!(message.getMessageType() == ChatMessage::MessageType::system) && !(message.getMessageType() == ChatMessage::MessageType::system_broadcast))
	{
		if(!auth->permitMessage(message))
		{
			logger->log("Message not delivered!");
			ChatMessage system_response_message(ChatMessage::MessageType::system, "", {message.getSender()}, "Your message was not delivered.");
			messageIncoming(system_response_message);
			return;
		}
	}

	if(message.getMessageType() == ChatMessage::MessageType::broadcast || (message.getMessageType() == ChatMessage::MessageType::system_broadcast))
	{
		for(auto i : connected)
		{
			i->postMessage(message);
		}
	}
	else
	{
		for(auto i : message.getReceivers())
		{
			auto user_connections = getUserConnections(i);
			for(auto j : user_connections)
			{
				j->postMessage(message);
			}
		}
		logger->log("Message incoming!");
	}
}

std::set <std::shared_ptr <Session>> BasicChat::getUserConnections(std::string username)
{
	std::set <std::shared_ptr <Session>> temp;
	for(auto i : connected)
	{
		if(i->getUser() == username)
			temp.insert(i);
	}
	return temp;
}

std::set <std::shared_ptr <Session>> BasicChat::getIpConnections(std::string ip)
{
	boost::system::error_code error;
	std::set <std::shared_ptr <Session>> temp;
	for(auto i : connected)
	{
		if(i->getConnection()->getRemoteIp() == ip)
			temp.insert(i);
	}
	return temp;
}
