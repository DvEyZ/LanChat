//TODO:
//Implement Auth() and containsBannedWords()

#include "Auth.h"

Auth::Auth()
{
	
}

IdentifyResponseMessage::Status Auth::authenticate(IdentifyMessage message)
{
	if(!require_account)	return true;
	else if(checkPassword(message.getUsername(), message.getPassword()))
		return IdentifyResponseMessage::Status.ok;
	else
		return IdentifyResponseMessage::Status.auth_failed_bad_credentials;
}

IdentifyResponseMessage::Status Auth::permitConnection(boost::shared_ptr <Connection> connection)
{
	boost::asio::system::error_code error;
	if(chat.getUserConnections(connection->user).size() >= max_connections_for_user)	
		return IdentifyResponseMessage::Status.conn_failed_too_many_for_user;
	if(chat.getIpConnections(connection->getSocket().remote_endpoint(error).address().to_string()).size() >= max_connections_for_ip)	
		return IdentifyResponseMessage::Status.conn_failed_too_many_for_ip;
	if(error)	return IdentifyResponseMessage::Status.fail_generic;
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