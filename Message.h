#pragma once

#include "helper_functions.cpp"
#include "defines.h"
#include <string>
#include <vector>

class Message
{
public:
	bool decodeHeader(std::vector <char> _header);
	virtual bool decodeBody(std::vector <char> _body) = 0;
	bool decodeAll(std::vector <char> _all);

	int getLength();
	
	virtual std::vector <char> encodeMessage() = 0;
protected:
	int length;
};