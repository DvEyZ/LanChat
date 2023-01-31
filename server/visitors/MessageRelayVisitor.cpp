#include "MessageRelayVisitor.h"

MessageRelayVisitor::MessageRelayVisitor(Chat* _chat, Auth* _auth, CommandProcessor* _cp)
    :chat(_chat), auth(_auth), cp(_cp)
{}

void MessageRelayVisitor::visitSendMessage(SendMessage* m)
{

}

void MessageRelayVisitor::visitCommandMessage(CommandMessage* m)
{
    
}