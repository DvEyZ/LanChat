//TODO:
//Implement Auth()

#include "Auth.h"

Auth::Auth()
{
	
}

IdentifyResponseMessage::Status Auth::authenticate(IdentifyMessage message)
{
	if(!require_account)	return IdentifyResponseMessage::Status::ok;
	else if(checkPassword(message.getUsername(), message.getPassword()))
		return IdentifyResponseMessage::Status::ok;
	else
		return IdentifyResponseMessage::Status::auth_failed_bad_credentials;
}

IdentifyResponseMessage::Status Auth::permitConnection(boost::shared_ptr <Connection> connection)
{
	boost::system::error_code error;
	if(chat->getUserConnections(connection->user).size() >= max_connections_for_user)	
		return IdentifyResponseMessage::Status::conn_failed_too_many_for_user;
	if(chat->getIpConnections(connection->socket().remote_endpoint(error).address().to_string()).size() >= max_connections_from_ip)	
		return IdentifyResponseMessage::Status::conn_failed_too_many_for_ip;
	if(banned_ips.contains(connection->socket().remote_endpoint(error).address().to_string()))
		return IdentifyResponseMessage::Status::conn_failed_ip_banned;
	if(error)	return IdentifyResponseMessage::Status::fail_generic;
	return IdentifyResponseMessage::Status::ok;
}

bool Auth::permitMessage(ChatMessage message)
{
	if(containsBannedWords(message.getMsgBody()))	return false;
	if((message.getMessageType() == ChatMessage::MessageType::broadcast) && (!isAllowedToBroadcast(message.getSender())))	return false;

	return true;
}

bool Auth::checkPassword(std::string user, std::string password)
{
	if(!require_password)	return true;
	if(user_list[user] == password)	return true;
	return false;
}

bool Auth::containsBannedWords(std::string message)
{
	for(auto i : banned_words)
	{
		if(message.contains(i)) return true;
	}
	return false;
}

bool Auth::isAllowedToBroadcast(std::string user)
{
	if(anyone_may_broadcast)	return true;
	if(broadcast_users.contains(user)) return true;
	else return false;
}