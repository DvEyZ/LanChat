#pragma once

#include <exception>

class SendMessage;
class CommandMessage;
class ResponseMessage;

class MessageVisitorException : public std::exception
{
public:
    const char* what()
    {
        return "Operation not supported on this type of message.";
    }
};

// The visitor methods throw an instance of MessageVisitorException if called on a wrong class.
class MessageVisitor
{
public:
    virtual void visitSendMessage(SendMessage* m);
    virtual void visitCommandMessage(CommandMessage* m);
    virtual void visitResponseMessage(ResponseMessage* m);
};