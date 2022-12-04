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

bool Message::decodeAll(std::vector <char> _all)
{
	bool dh = decodeHeader(std::vector(_all.begin(), _all.begin() + MESSAGE_HEADER_LENGTH));
	bool db = decodeBody(std::vector(_all.begin() + MESSAGE_HEADER_LENGTH, _all.end()));

	return dh && db;
}

int Message::getLength()
{
	return length;
}