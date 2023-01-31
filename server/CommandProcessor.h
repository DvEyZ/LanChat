#pragma once

#include "../message/CommandMessage.h"
#include "../message/ResponseMessage.h"

class CommandProcessor
{
public:
    virtual ResponseMessage executeCommand(CommandMessage& m);
};
