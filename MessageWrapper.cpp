#include "MessageWrapper.h"

MessageWrapper::MessageWrapper(Message& m)
{
    data = m.encode();
    length = data.size();
}

void MessageWrapper::decode(std::vector <char> d)
{
    auto len_part = std::vector(d.begin(), d.begin() + sizeof(uint32_t));
    d.erase(d.begin(), d.begin() + sizeof(uint32_t));
    decodeHeader(len_part);
    decodeBody(d);
}

void MessageWrapper::decodeHeader(std::vector <char> d)
{
    memmove(&length, d.data(), sizeof(uint32_t));
}

void MessageWrapper::decodeBody(std::vector <char> d)
{
    data = std::string(d.begin(), d.end());
}

uint32_t MessageWrapper::getLength()
{
    return length;
}

std::string MessageWrapper::getData()
{
    return data;
}

std::vector <char> MessageWrapper::encode()
{
    std::vector <char> len(4);
    memmove(len.data(), &length, sizeof(uint32_t));

    std::vector <char> dat(data.begin(), data.end());

    std::vector <char> sum;
    sum.insert(sum.end(), len.begin(), len.end());
    sum.insert(sum.end(), sum.begin(), sum.end());

    return sum;
}