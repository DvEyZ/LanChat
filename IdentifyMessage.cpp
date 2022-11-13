//TODO:
//Implement decodeHeader(), decodeBody(), encodeMessage(), getLength(), getUsername(), getPassword()

#include "IdentifyMessage.h"

IdentifyMessage::IdentifyMessage(std::string _username, std::string _password)
	:username(_username), password(_password), length(_username.length())
{}

bool IdentifyMessage::decodeHeader(std::vector<char> _header)
{

}