#pragma once

#include "../../message/MessageVisitor.h"
#include "../Chat.h"
#include "../Auth.h"
#include "../CommandProcessor.h"
#include "../../message/ResponseMessage.h"

class MessageRelayVisitor : public MessageVisitor
{
public:
    MessageRelayVisitor(Chat* chat, Auth* auth, CommandProcessor* cp);
    void visitSendMessage(SendMessage* m);
    void visitCommandMessage(CommandMessage* m);
    void visitResponseMessage(ResponseMessage* m);

    ResponseMessage getResponse();
private:
    Chat* chat;
    Auth* auth;
    CommandProcessor* cp;

    ResponseMessage result;
};