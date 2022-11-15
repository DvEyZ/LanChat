//FINISHED

#pragma once

#include "Message.h"

class IdentifyMessage : public Message
{
public:
	IdentifyMessage();
	IdentifyMessage(std::string _username, std::string _password);

	bool decodeBody(std::vector<char> _body);
	std::vector<char> encodeMessage();
	std::string getUsername();
	std::string getPassword();
private:
	std::string username;
	std::string password;
};