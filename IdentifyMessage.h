//FINISHED

#pragma once

#include "helper_functions.h"

class IdentifyMessage
{
public:
	IdentifyMessage();
	IdentifyMessage(std::string _username, std::string _password);
	bool decodeHeader(char* header);
	bool decodeBody(char* body);
	char* encode();
	int getLength();
	std::string getUsername();
	std::string getPassword();
private:
	int length;
	std::string username;
	std::string password;
};