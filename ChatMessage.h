//FINISHED

#pragma once

#include "Message.h"

class ChatMessage : public Message
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
	MessageType getMessageType();
	int getRecvNum();
	std::vector <std::string> getReceivers();
	std::string getSender();
	std::string getMsgBody();
	std::vector<char> encodeMessage();
	//bool decodeHeader(std::vector<char> _header);
	bool decodeBody(std::vector<char> _body);
protected:
	int recv_num;
	std::string sender;
	std::vector <std::string> receivers;
	std::string body;
	MessageType message_type;
};