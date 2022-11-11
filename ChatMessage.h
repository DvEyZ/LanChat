//FINISHED

#pragma once

#include <string>
#include <vector>
#include "helper_functions.h"

class ChatMessage
{
	int body_length;
	int recv_num;
	std::string sender;
	std::vector <std::string> receivers;
	std::string msgBody;
public:
	enum MessageType {
		unicast = 'u',
		broadcast = 'b',
		system = 's',
		system_broadcast = 'v'
	} message_type;
	ChatMessage(MessageType _type, std::string _sender, std::vector <std::string> _receivers, std::string _body);
	~ChatMessage();
	int getBodyLength();
	MessageType getMessageType();
	int getRecvNum();
	std::vector <std::string> getReceivers();
	std::string getSender();
	std::string getMsgBody();
	char* encodeMessage();
	bool decodeHeader(char* header);
	bool decodeBody(char* body);
};