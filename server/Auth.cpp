#include "Auth.h"

void Auth::addToChat(Chat* _chat)
{
	_chat->auth = this;
	chat = _chat;
}