//FINISHED

#include "ChatMessage.h"

ChatMessage::ChatMessage(MessageType _type, std::string _sender, std::vector <std::string> _receivers, std::string _body)
    :message_type(_type), sender(_sender), receivers(_receivers), body(_body)
{
    body_length = body.length() + 1;
    body_length += sender.length() + 1;
    for(auto i : receivers)
    {
        body_length += i.length() + 1;
    }
}

ChatMessage::ChatMessage()
{}

ChatMessage::~ChatMessage() {}

int ChatMessage::getBodyLength()
{
    return body_length;
}

ChatMessage::MessageType ChatMessage::getMessageType()
{
    return message_type;
}

int ChatMessage::getRecvNum()
{
    return recv_num;
}

std::vector <std::string> ChatMessage::getReceivers()
{
    return receivers;
}

std::string ChatMessage::getSender()
{
    return sender;
}

std::string ChatMessage::getMsgBody()
{
    return body;
}

/*
LLLLtnnn rr0rr0rr0mmmmm
 HEADER       BODY
*/
std::vector<char> ChatMessage::encodeMessage()
{
    std::string message_composed;
    message_composed += intts(body_length, 4);          // Body length
    message_composed += message_type;                   // Message type
    message_composed += intts(receivers.size(), 3);     // Number of receivers
    message_composed += sender;
    message_composed += "\0";
    for(auto i : receivers)
    {
        message_composed += i;
        message_composed += "\0";
    }
    message_composed += body;
    return std::vector<char>(message_composed.begin(), message_composed.end());
}

bool ChatMessage::decodeHeader(std::vector<char> header)    // true on success, false on fail
{
    if(header.size() != 8)   return false;
    if(std::stoi(std::string(header.begin(), header.begin() + 4)) == 0)
    {
        return false;
    }  
    if(static_cast<MessageType>(header.at(5)) != MessageType::unicast && static_cast<MessageType>(header.at(5)) != MessageType::broadcast && static_cast<MessageType>(header.at(5)) != MessageType::system && static_cast<MessageType>(header.at(5)) != MessageType::system_broadcast)    
    {
        return false;
    }
    if(std::stoi(std::string(header.begin() + 6, header.end())) == 0 && static_cast<MessageType>(header.at(5)) != MessageType::broadcast)
    {
        return false;
    }
    body_length = std::stoi(std::string(header.begin(), header.begin() + 4));
    message_type = static_cast<MessageType>(header.at(5));
    recv_num = std::stoi(std::string(header.begin() + 6, header.end()));
    return true;
}

bool ChatMessage::decodeBody(std::vector<char> _body)
{
    std::string temp(_body.begin(), _body.end());
    sender = temp.substr(0, temp.find('\0'));
    temp.erase(0, temp.find('\0') + 1);

    for(int i = 0; i < recv_num; i++)
    {
        receivers.push_back(temp.substr(0, temp.find('\0')));
        temp.erase(0, temp.find('\0') + 1);
    }

    body = temp.substr(0, temp.find('\0'));
    temp.erase(0, temp.find('\0') + 1);

    if(message_type != MessageType::system && message_type != MessageType::system_broadcast && sender == "") return false;    // sender can't be empty unless message is system message
    if(message_type == MessageType::system || message_type != MessageType::system_broadcast && sender != "") return false;    // sender must be empty if message is system message

    if(receivers.size() != recv_num) return false;  // this means that broadcast message must have recv_num of 0

    body = temp;
    return true;
}