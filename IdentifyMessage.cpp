//TODO:
//Implement decodeHeader(), decodeBody(), encodeMessage(), getLength(), getUsername(), getPassword()

#include "IdentifyMessage.h"

IdentifyMessage::IdentifyMessage(std::string _username, std::string _password)
	:username(_username), password(_password)
{
	length = username.length() + password.length() + 2;
}

std::string IdentifyMessage::getUsername()
{
	return username;
}

std::string IdentifyMessage::getPassword()
{
	return password;
}

bool IdentifyMessage::decodeBody(std::vector<char> _body)
{
	std::string temp(_body.begin(), _body.end());
	std::string temp_username;
	std::string temp_password;

	try
	{
		temp_username = temp.substr(0, temp.find('\0'));
        temp.erase(0, temp.find('\0') + 1);

		temp_password = temp.substr(0, temp.find('\0'));
        temp.erase(0, temp.find('\0') + 1);
	}
	catch(std::out_of_range)
	{
		return false;
	}

	username = temp_username;
	password = temp_password;

	return true;
}