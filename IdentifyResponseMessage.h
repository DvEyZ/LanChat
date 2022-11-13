#pragma once

#include "helper_functions.h"

class IdentifyResponseMessage
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
	IdentifyResponseMessage(Status status);
	IdentifyResponseMessage();
	
	bool decodeHeader(std::vector<char> _header);
	bool decodeBody(std::vector<char> _body);
	std::vector<char> encodeMessage();
	int getStatus();
private:
	Status status;
};