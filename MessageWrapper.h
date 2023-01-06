#pragma once

#include "message/Message.h"

class Message;

class MessageWrapper
{
public:
    MessageWrapper() {};
    MessageWrapper(Message&);
    void decode(std::vector <char> data);
    void decodeHeader(std::vector <char> data);
    void decodeBody(std::vector <char> data);
    uint32_t getLength();
    std::string getData();
    std::vector <char> encode();
private:
    uint32_t length;
    std::string data;
};