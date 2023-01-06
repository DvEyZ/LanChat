#include "Signed.h"

Signed::Signed(std::string s)
    :sender(s)
{}

std::string Signed::getSender()
{
    return sender;
}