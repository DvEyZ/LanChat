//FINISHED

#include "ChatMessage.h"

ChatMessage::ChatMessage(MessageType _type, std::string _sender, std::vector <std::string> _receivers, std::string _body)
    :message_type(_type), sender(_sender), receivers(_receivers), body(_body)
{
    length = body.length() + 1;
    length += sender.length() + 1;
    for(auto i : receivers)
    {
        length += i.length() + 1;
    }
}

ChatMessage::ChatMessage()
{}


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
    message_composed += intts(length, MESSAGE_HEADER_LENGTH);          // Body length
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

bool ChatMessage::decodeBody(std::vector<char> _body)
{
    std::string temp(_body.begin(), _body.end());
    int temp_message_type;
    int temp_recv_num;
    std::string temp_sender;
    std::vector <std::string> temp_receivers;
    std::string temp_body;
    try
    {
        if(temp.at(0) != MessageType::unicast && temp.at(0) != MessageType::broadcast && temp.at(0) != MessageType::system && temp.at(0) != MessageType::system_broadcast)
            return false;
        temp_message_type = static_cast<MessageType>(temp.at(0));
        temp.erase(0);

        temp_recv_num = stoi(temp.substr(0,3));
        temp.erase(0,3);

        temp_sender = temp.substr(0, temp.find('\0'));
        temp.erase(0, temp.find('\0') + 1);

        for(int i = 0; i < recv_num; i++)
        {
            temp_receivers.push_back(temp.substr(0, temp.find('\0')));
            temp.erase(0, temp.find('\0') + 1);
        }

        temp_body = temp.substr(0, temp.find('\0'));
        temp.erase(0, temp.find('\0') + 1);
    }
    catch(std::out_of_range)
    {
        return false;
    }

    if(temp_message_type != MessageType::system && temp_message_type != MessageType::system_broadcast && temp_sender == "") return false;    // sender can't be empty unless message is system message
    if(temp_message_type == MessageType::system || temp_message_type != MessageType::system_broadcast && temp_sender != "") return false;    // sender must be empty if message is system message

    if(temp_receivers.size() != temp_recv_num) return false;  // this means that broadcast message must have recv_num of 0
    
    message_type = static_cast<MessageType>(temp_message_type);
    recv_num = temp_recv_num;
    sender = temp_sender;
    receivers = temp_receivers;
    body = temp_body;

    return true;
}