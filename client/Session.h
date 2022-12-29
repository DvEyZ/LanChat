#pragma once

#include <string>
#include <functional>
#include "../Connection.h"
#include "../SocketConnectionError.h"
#include "../ChatMessage.h"


class Session
{
    Session(App app, std::shared_ptr <Connection> connection);

    std::shared_ptr <Connection> getConnection();
    void identify(std::string username, std::string password);
    void sendMessage(ChatMessage message);
    void recvMessage();
    void disconnect();
private:
    App* app;

    void onSendMessage();
    void onRecvMessage(ChatMessage message);
    void onError();
    std::shared_ptr <Connection> connection;
};