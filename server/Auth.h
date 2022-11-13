//FINISHED

#pragma once

#include <map>
#include <experimental/optional>
#include "defines.h"
#include "Chat.h"
#include "../IdentifyMessage.h"
#include "../IdentifyResponseMessage.h"

class Auth
{
public:
	Auth();
	IdentifyResponseMessage::Status authenticate(IdentifyMessage identification);					//check credentials
	IdentifyResponseMessage::Status permitConnection(boost::shared_ptr <Connection> connection);	//check if connection is permitted
	bool permitMessage(ChatMessage message);							//check if message is permitted
private:
	Chat* chat;
//rules for Auth::authenticate()
	bool require_account;
	std::experimental::optional <bool> require_password;
	std::map <std::string, std::string> user_list;

	bool checkPassword(std::string user, std::string password);
//rules for Auth::permitConnection()
	std::experimental::optional <int> max_connections_from_ip;	
	std::experimental::optional <int> max_connections_for_user;	//if none, any number of connections is permitted
	std::set <std::string> banned_ips;				//list of banned IP addresses
//rules for Auth::permitMessage()
	bool anyone_may_broadcast;
	std::set <std::string> banned_words;
	std::set <std::string> broadcast_users;

	bool containsBannedWords(std::string text);
	bool isAllowedToBroadcast(std::string user);
};
