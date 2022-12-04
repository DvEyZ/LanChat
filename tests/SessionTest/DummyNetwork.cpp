#include "DummyNetwork.h"

void DummyNetwork::write(std::string text)
{
	buf << text;
}

std::string DummyNetwork::read(int how_much)
{
	std::vector<char> buffer(how_much);
	buf.read(&buffer[0], buffer.size());
	std::string result(buffer.begin(), buffer.end());

	return result;
}