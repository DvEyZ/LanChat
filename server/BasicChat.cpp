//FINISHED

#include "BasicChat.h"

BasicChat::BasicChat(Auth* _auth)
{
	this->auth = _auth;
}

void BasicChat::log(std::string message)
{
	std::cout << message;
	logstream.open(LOG_FILE_SERVER);
	logstream << message;
	logstream.close();
}

void BasicChat::join(boost::shared_ptr <Connection> connection)
{
	connected.insert(connection);
}

void BasicChat::leave(boost::shared_ptr <Connection> connection)
{
	connected.erase(connection);
}

void BasicChat::messageIncoming(ChatMessage message)
{
	// system messages don't need permission from Auth
	if(!message.getMessageType() == ChatMessage::MessageType::system && !message.getMessageType() == ChatMessage::MessageType::system_broadcast)
	{
		if(!auth->permitMessage(message))
		{
			ChatMessage system_response_message(ChatMessage::MessageType::system, "", {message.getSender()}, "Your message was not delivered.");
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
	}
}

std::set <boost::shared_ptr <Connection>> BasicChat::getUserConnections(std::string username)
{
	std::set <boost::shared_ptr <Connection>> temp;
	for(auto i : connected)
	{
		if(i->getUser() == username)
			temp.insert(i);
	}
	return temp;
}

std::set <boost::shared_ptr <Connection>> BasicChat::getIpConnections(std::string ip)
{
	boost::system::error_code error;
	std::set <boost::shared_ptr <Connection>> temp;
	for(auto i : connected)
	{
		if(i->socket().remote_endpoint(error).address().to_string() == ip)
			temp.insert(i);
	}
	return temp;
}
