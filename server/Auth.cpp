//TODO:
//Implement Auth() and containsBannedWords()

#include "Auth.h"

Auth::Auth()
{
	
}

bool Auth::authenticate(IdentifyMessage message)
{
	if(message.getUsername == system_message_user)	return false;
	if(!require_account)	return true;
	else return checkPassword(message.getUsername(), message.getPassword());
}

bool Auth::permitConnection(boost::shared_ptr <Connection> connection)
{
	boost::asio::system::error_code error;
	if(chat.getUserConnections(connection->user).size() >= max_connections_for_user)	return false;
	if(chat.getIpConnections(connection->getSocket().remote_endpoint(error).address().to_string()).size() >= max_connections_for_ip)	return false;
	if(error)	return false;
	return true;
}

bool Auth::permitMessage(ChatMessage message)
{
	if(containsBannedWords(message.getMsgBody()))	return false;
	if((message.getMessageType() == ChatMessage::MessageType.broadcast) && (!isAllowedToBroadcast(message.getSender())))	return false;
	return true;
}

bool checkPassword(std::string user, std::string password)
{
	if(!require_password)	return true;
	if(user_list[user] == password)	return true;
	return false;
}

bool isAllowedToBroadcast(std::string user)
{
	if(anyone_may_broadcast)	return true;
	if(broadcast_users.contains(user)) return true;
	else return false;
}