#include "TestConnection.h"

void TestConnection::read(std::function<void(std::vector <char>)> callback)
{
	std::string temp = network->read(MESSAGE_HEADER_LENGTH);
	int msg_len = std::stoi(temp);
	temp += network->read(msg_len);
	callback(std::vector(temp.begin(), temp.end()));
}

void TestConnection::write(std::vector <char> text, std::function<void ()> callback)
{
	network->write(std::string(text.begin(), text.end()));
}

std::string TestConnection::getRemoteIp()
{
	return "127.0.0.1";
}