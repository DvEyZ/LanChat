#pragma once

#include "Message.h"
#include "helper_functions.h"

class IdentifyResponseMessage : public Message
{
public:
	enum Status {
		ok = 0,
		auth_failed_malformed = 1,
		auth_failed_bad_credentials = 2,
		conn_failed_too_many_for_user = 3,
		conn_failed_too_many_for_ip = 4,
		conn_failed_ip_banned = 5,
		fail_generic = 9
	};
	IdentifyResponseMessage(Status _status);
	IdentifyResponseMessage();
	
	bool decodeBody(std::vector<char> _body);
	std::vector<char> encodeMessage();

	Status getStatus();
private:
	Status status;
};