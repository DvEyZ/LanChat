#include "TestConnection.h"

TestConnection::TestConnection(std::stringstream* _sstream)
	:sstream(_sstream)
{}

void TestConnection::read(std::function<void(std::vector <char>)> callback)
{
	std::string temp;
	char header_buffer[4];	//niestety nie ma lepszej opcji
	int size;

	sstream->get(header_buffer, 4);
	temp = std::string(header_buffer);
	try 
	{
		size = std::stoi(temp);
	}
	catch(std::invalid_argument)
	{
		throw new ConnectionException("Invalid header");
	}

	std::unique_ptr <char> body_buffer (new char[size]);
	
	sstream->get(body_buffer.get(), size);

	temp += std::string(body_buffer.get());

	callback(std::vector(temp.begin(), temp.end()));
}

void TestConnection::write(std::vector <char> text, std::function<void ()> callback)
{
	*sstream << std::string(text.begin(), text.end());
	callback();
}

std::string TestConnection::getRemoteIp()
{
	return "127.0.0.1";
}