#pragma once

#include "message/Message.h"

class MessageWrapper
{
public:
    MessageWrapper(Message& m);
    Message& getMessage();

    std::vector <char> encode();
    bool decode(std::vector <char> data);
private:
    int length;
    std::string data; 
};
