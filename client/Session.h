#pragma once

#include <string>
#include <functional>
#include "../Connection.h"
#include "../SocketConnectionError.h"
#include "../ChatMessage.h"
#include "../IdentifyMessage.h"
#include "../IdentifyResponseMessage.h"

#include "App.h"

class App;

class Session
{
public:
    Session(App* app, std::shared_ptr <Connection> connection);
    ~Session();
    
    std::shared_ptr <Connection> getConnection();
    void identify(std::string username, std::string password, std::function < void(IdentifyResponseMessage) > callback);
    void sendUnicastMessage(std::vector <std::string> receivers, std::string message);
    void sendBroadcastMessage(std::string message);
    void recvMessage(std::function <void(ChatMessage)> callback);
    std::string getUsername();
private:
    App* app;
    std::string username;
    std::shared_ptr <Connection> connection;
};