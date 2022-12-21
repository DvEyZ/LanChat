#include "Connection.h"

void Connection::setErrorCallback(std::function <void(SocketConnectionError)> callback)
{
    error_callback = callback;
}