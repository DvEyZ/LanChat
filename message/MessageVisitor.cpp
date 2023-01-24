#include "MessageVisitor.h"

void MessageVisitor::visitSendMessage(SendMessage *m)
{
    throw new MessageVisitorException;
}

void MessageVisitor::visitCommandMessage(CommandMessage *m)
{
    throw new MessageVisitorException;
}

void MessageVisitor::visitResponseMessage(ResponseMessage *m)
{
    throw new MessageVisitorException;
}

// This behavior is meant to be overridden in all supported cases.