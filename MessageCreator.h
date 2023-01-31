#pragma once

#include "MessageWrapper.h"

class MessageCreator
{
public:
    MessageCreator(MessageWrapper w);
    Message& get();
};
