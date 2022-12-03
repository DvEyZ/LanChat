#include "Message.h"

bool Message::decodeHeader(std::vector <char> _header)
{
	if(_header.size() != 4) return false;
	try
	{
		length = stoi(std::string(_header.begin(), _header.end()));
	}
	catch(std::invalid_argument)
	{
		return false;
	}
	return true;
}

int Message::getLength()
{
	return length;
}