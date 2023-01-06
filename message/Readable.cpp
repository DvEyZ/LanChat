#include "Readable.h"

Readable::Readable(ReadableMessageBody b)
    :body(b)
{}

ReadableMessageBody Readable::getBody()
{
    return body;
}