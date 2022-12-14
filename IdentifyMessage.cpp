//FINISHED

#include "IdentifyMessage.h"

IdentifyMessage::IdentifyMessage(std::string _username, std::string _password)
	:username(_username), password(_password)
{
	length = username.length() + password.length() + 1;
}

IdentifyMessage::IdentifyMessage()
{}

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
		temp_username = temp.substr(0, temp.find(MESSAGE_FIELD_SEPARATOR));
        temp.erase(0, temp.find(MESSAGE_FIELD_SEPARATOR) + 1);

		temp_password = temp.substr(0, temp.find(MESSAGE_FIELD_SEPARATOR));
        temp.erase(0, temp.find(MESSAGE_FIELD_SEPARATOR));
	}
	catch(std::out_of_range)
	{
		return false;
	}

	username = temp_username;
	password = temp_password;

	return true;
}

std::vector <char> IdentifyMessage::encodeMessage()
{
	std::string temp;
	temp += intts(length, MESSAGE_HEADER_LENGTH);
	temp += username + MESSAGE_FIELD_SEPARATOR;
	temp += password;
	return std::vector <char> (temp.begin(), temp.end());
}