#pragma once

#include <map>
#include "defines.h"
#include "Chat.h"
#include "../IdentifyMessage.h"

class Auth
{
public:
	Auth();
	bool authenticate(IdentifyMessage identification);					//check credentials
	bool permitConnection(boost::shared_ptr <Connection> connection);	//check if connection is permitted
	bool permitMessage(ChatMessage message);							//check if message is permitted
private:
	Chat* chat;
	std::string system_message_user;
//rules for Auth::authenticate()
	bool require_account;
	bool require_password;
	std::map <std::string, std::string> user_list;

	bool checkPassword(std::string user, std::string password);
//rules for Auth::permitConnection()
	int max_connections_from_ip;
	int max_connections_for_user;
//rules for Auth::permitMessage()
	bool anyone_may_broadcast;
	std::set <std::string> banned_words;
	std::set <std::string> broadcast_users;

	bool containsBannedWords(std::string text);
	bool isAllowedToBroadcast(std::string user);
}