//TODO:
//Implement Auth()

#include "BasicAuth.h"

BasicAuth::BasicAuth()
{
	// get user list
	std::ifstream config_file(CONFIG_FILE_SERVER);
	nlohmann::json config;
	if(!config_file.good())
	{
		useDefaultConfig();
		return;
	}
	try
	{
		config = nlohmann::json::parse(config_file);
	}
	catch(nlohmann::json::exception)
	{
		useDefaultConfig();
		return;
	}
	require_account = config["require_account"].get<bool>();
}

void BasicAuth::useDefaultConfig()
{
	require_account = false;
	banned_ips = {};
	anyone_may_broadcast = true;
	banned_words = {};
	broadcast_users = {};
}

IdentifyResponseMessage::Status BasicAuth::authenticate(IdentifyMessage message)
{
	if(!require_account)	return IdentifyResponseMessage::Status::ok;
	else if(checkPassword(message.getUsername(), message.getPassword()))
		return IdentifyResponseMessage::Status::ok;
	else
		return IdentifyResponseMessage::Status::auth_failed_bad_credentials;
}

IdentifyResponseMessage::Status BasicAuth::permitConnection(boost::shared_ptr <Connection> connection)
{
	boost::system::error_code error;
	try
	{
	if(chat->getUserConnections(connection->getUser()).size() >= max_connections_for_user.value())	
		return IdentifyResponseMessage::Status::conn_failed_too_many_for_user;
	}
	catch(std::bad_optional_access)
	{}
	try
	{
	if(chat->getIpConnections(connection->socket().remote_endpoint(error).address().to_string()).size() >= max_connections_from_ip.value())	
		return IdentifyResponseMessage::Status::conn_failed_too_many_for_ip;
	}
	catch(std::bad_optional_access)
	{}
	if(banned_ips.contains(connection->socket().remote_endpoint(error).address().to_string()))
		return IdentifyResponseMessage::Status::conn_failed_ip_banned;
	if(error)	return IdentifyResponseMessage::Status::fail_generic;
	return IdentifyResponseMessage::Status::ok;
}

bool BasicAuth::permitMessage(ChatMessage message)
{
	if(containsBannedWords(message.getMsgBody()))	return false;
	if((message.getMessageType() == ChatMessage::MessageType::broadcast) && (!isAllowedToBroadcast(message.getSender())))	return false;

	return true;
}

bool BasicAuth::checkPassword(std::string user, std::string password)
{
	if(!require_password)	return true;
	if(user_list[user] == password)	return true;
	return false;
}

bool BasicAuth::containsBannedWords(std::string message)
{
	for(auto i : banned_words)
	{
		if(string_contains(message, i)) return true;
	}
	return false;
}

bool BasicAuth::isAllowedToBroadcast(std::string user)
{
	if(anyone_may_broadcast)	return true;
	if(broadcast_users.contains(user)) return true;
	else return false;
}