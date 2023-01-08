#pragma once

#include "../SendMessage.h"
#include "../ResponseMessage.h"
#include "../IdentifyMessage.h"

class IMessageVisitor
{
public:
    virtual void visitSendMessage(SendMessage& m) = 0;
    virtual void visitResponseMessage(ResponseMessage& m) = 0;
    virtual void visitAddressedResponseMessage(AddressedResponseMessage& m) = 0;
    virtual void visitIdentifyMessage(IdentifyMessage& m) = 0;
};
