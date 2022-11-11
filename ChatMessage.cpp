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

ChatMessage::~ChatMessage() {}

int ChatMessage::getBodyLength()
{
    return body_length;
}

MessageType ChatMessage::getMessageType()
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
    return msgBody;
}

/*
LLLLtnnn rr0rr0rr0mmmmm
 HEADER       BODY
*/
char* ChatMessage::composeMessage()
{
    std::string message_composed;
    message_composed += intts(body_length, 4);          // Body length
    message_composed += message_type;                   // Message type
    message_composed += intts(receivers.size(), 3);     // Number of receivers
    message_composed += sender;
    message_composed += "\x00";
    for(auto i : receivers)
    {
        message_composed += i;
        message_composed += "\x00";
    }
    message_composed += body;
    return message_composed.c_str();
}

bool ChatMessage::decodeHeader(char* header)    // true on success, false on fail
{
    if(sizeof(char) != 8)   return false;
    char* _enc_length = new char[4];    // max length 9999
    char* _enc_type = new char;         // type
    char* _enc_recv_num = new char[3];  // max number of receivers 999
    memcpy(_enc_length, header, sizeof(_enc_length));
    memcpy(_enc_type, header+4, sizeof(_enc_type));
    memcpy(_enc_recv_num, header+5, sizeof(_enc_recv_num));
    
    if(atoi(body_length) == 0)
    {
        delete [] _enc_length;
        delete _enc_type;
        delete [] _enc_recv_num;
        return false;
    }  
    if(_enc_type != MessageType.unicast && _enc_type != MessageType.broadcast && _enc_type != MessageType.system && _enc_type != MessageType.system_broadcast)    
    {
        delete [] _enc_length;
        delete _enc_type;
        delete [] _enc_recv_num;
        return false;
    }
    if(atoi(_enc_recv_num) == 0 && _enc_type != MessageType.broadcast)
    {
        delete [] _enc_length;
        delete _enc_type;
        delete [] _enc_recv_num;
        return false;
    }
    body_length = atoi(_enc_length);
    message_type = _enc_type;
    recv_num = atoi(_enc_recv_num);

    delete [] _enc_length;
    delete _enc_type;
    delete [] _enc_recv_num;

    return true;
}

bool ChatMessage::decodeBody(char* body)
{
    char* pointer = body;

    std::string temp;
    while(*pointer != '\x00')
    {
        temp += pointer;
        pointer++;
        if(pointer - body > body_length) return false;
    }
    sender = temp;
    pointer++;

    if(message_type != MessageType.system && message_type != MessageType.system_broadcast && sender == "") return false;    // sender can't be empty unless message is system message
    if(message_type == MessageType.system || message_type != MessageType.system_broadcast && sender != "") return false;    // sender must be empty if message is system message

    for(int i = 0; i < recv_num; i++)
    {
        std::string temp;
        while(*pointer != '\x00')
        {
            temp += *pointer;
            pointer++;
            if(pointer - body > body_length)
            return false;
        }
        pointer++;
        receivers.push_back(temp);
        temp = "";
    }
    
    temp = "";
    while(*pointer != '\x00')
    {
        temp += pointer;
        pointer++;
        if(pointer - body > body_length) return false;
    }
    
    if(receivers.size() != recv_num) return false;  // this means that broadcast message must have recv_num of 0

    msgBody = temp;
    return true;
}