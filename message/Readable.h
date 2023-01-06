#pragma once
#include "ReadableMessageBody.h"

class Readable
{
public:
    Readable(ReadableMessageBody b);
    ReadableMessageBody getBody();
private:
    ReadableMessageBody body;
};
