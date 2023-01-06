#include "ReadableMessageBody.h"

ReadableMessageBody::ReadableMessageBody(std::string c, std::string t)
    :content(c), type(t)
{}

std::string ReadableMessageBody::getContent()
{
    return content;
}

std::string ReadableMessageBody::getType()
{
    return type;
}