//TODO:
//Implement decodeHeader(), decodeBody(), encodeMessage(), getLength(), getUsername(), getPassword()

#include "IdentifyMessage.h"

IdentifyMessage::IdentifyMessage(std::string _username, std::string _password)
	:username(_username), password(_password), length(_username.length())
{}

std::string IdentifyMessage::getUsername()
{
	return username;
}

std::string IdentifyMessage::getPassword()
{
	return password;
}

bool IdentifyMessage::decodeBody(std::vector<char> _header)
{

}