#include "Auth.h"

void Auth::addToChat(Chat* chat)
{
	chat->auth = this;
}