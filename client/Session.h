#pragma once

#include <string>
#include <functional>
#include "../Connection.h"
#include "../SocketConnectionError.h"
#include "../ChatMessage.h"


class Session
{
    Session(std::shared_ptr <Connection> connection);

    std::shared_ptr <Connection> getConnection();

    void setErrorHandler();
    void identify(std::string username, std::string password);
    void sendMessage(ChatMessage message);
    void recvMessage(std::function <void(ChatMessage)> callback);
private:

    std::function <void(ChatMessage)> onRecvMessage();
    void onError();
    std::shared_ptr <Connection> connection;
};