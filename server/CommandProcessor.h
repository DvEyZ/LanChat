#pragma once

#include "../message/CommandMessage.h"
#include "../message/ResponseMessage.h"

class CommandProcessor
{
public:
    ResponseMessage executeCommand(CommandMessage& m);
};
