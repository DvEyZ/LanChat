//TODO:
//Implement decodeHeader(), decodeBody(), encode(), getLength(), getUsername(), getPassword()

#include "IdentifyMessage.h"

IdentifyMessage::IdentifyMessage(std::string _username, std::string _password)
	:username(_username), password(_password), username_length(_username.length())
{}