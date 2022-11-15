#include "IdentifyResponseMessage.h"

IdentifyResponseMessage::IdentifyResponseMessage(Status _status)
	:status(_status)
{
	length = MESSAGE_HEADER_LENGTH + 1;
}

IdentifyResponseMessage::IdentifyResponseMessage()
{}

IdentifyResponseMessage::Status IdentifyResponseMessage::getStatus()
{
	return status;
}

bool IdentifyResponseMessage::decodeBody(std::vector<char> _body)
{
	if(_body.size() != 1) return false;
	status = static_cast <Status> (_body.at(0));
	return true;
}

std::vector<char> IdentifyResponseMessage::encodeMessage()
{
	std::string temp;
	temp += intts(length, 4) + intts(status, 1);
	
	return(std::vector <char> (temp.begin(), temp.end()));
}

