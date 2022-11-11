#pragma once
#include "helper_functions.h"

class IdentifyMessage
{
public:
	IdentifyMessage(std::string _username);
	bool decodeHeader();
	bool decodeBody();
	char* encode();
	std::string getUsername();
	std::string getPassword();
private:
	int username_length;
	std::string username;
	std::string password;
};