// FINISHED

#include "Chat.h"

void Chat::log(std::string message)
{
	std::cout << message;
	logstream.open(LOG_FILE_SERVER);
	logstream << message;
	logstream.close();
}

void Chat::join(boost::shared_ptr <Connection> connection)
{
	connected.insert(connection);
}

void Chat::leave(boost::shared_ptr <Connection> connection)
{
	connected.erase(connection);
}

void Chat::messageIncoming(ChatMessage message)
{
	if(message.getMessageType() == message.message_type.broadcast)
	{
		for(auto i : connected)
		{
			i.postMessage(message);
		}
	}
	else
	{
		for(auto i : message.getReceivers())
		{
			auto user_connections = getUserConnections(i);
			for(auto j : user_connections)
			{
				j.postMessage(message);
			}
		}
	}
}