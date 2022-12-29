#pragma once

#include <string>
#include <functional>
#include "../Connection.h"
#include "../SocketConnectionError.h"
#include "../ChatMessage.h"
#include "../IdentifyResponseMessage.h"


class Session
{
public:
    Session(App* app, std::shared_ptr <Connection> connection);

    std::shared_ptr <Connection> getConnection();
    void identify(std::string username, std::string password, std::function < void(IdentifyResponseMessage) > callback);
    void sendUnicastMessage(std::vector <std::string> receivers, std::string message);
    void sendBroadcastMessage(std::string message);
    void recvMessage(std::function <void(ChatMessage)> callback);
    void disconnect(std::function <void(void)> callback);
    std::string getUsername();
private:
    App* app;
    std::string username;

    void onSendMessage();
    void onRecvMessage(ChatMessage message);
    void onError();

    std::shared_ptr <Connection> connection;
};