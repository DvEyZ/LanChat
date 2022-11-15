#pragma once

#include "helper_functions.cpp"
#include <string>
#include <vector>

class Message
{
public:
	bool decodeHeader(std::vector <char> _header);
	virtual bool decodeBody(std::vector <char> _body) = 0;

	int getLength();
	
	virtual std::vector <char> encodeMessage() = 0;
protected:
	int length;
};