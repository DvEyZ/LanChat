//FINISHED

#pragma once

#include <string>
#include <vector>
#include "helper_functions.h"

class ChatMessage
{
public:
	enum MessageType {
		unicast = 'u',
		broadcast = 'b',
		system = 's',
		system_broadcast = 'v'
	};
	ChatMessage(MessageType _type, std::string _sender, std::vector <std::string> _receivers, std::string _body);
	ChatMessage();
	~ChatMessage();
	int getBodyLength();
	MessageType getMessageType();
	int getRecvNum();
	std::vector <std::string> getReceivers();
	std::string getSender();
	std::string getMsgBody();
	char* encodeMessage();
	bool decodeHeader(char* _header);
	bool decodeBody(char* _body);
private:
	int body_length;
	int recv_num;
	std::string sender;
	std::vector <std::string> receivers;
	std::string body;
	MessageType message_type;
};