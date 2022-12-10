#include "SocketConnection.h"

SocketConnection::SocketConnection(boost::asio::io_context& iocontext)
	:socket(iocontext), malformed_messages(0), read_buffer()
{

}

SocketConnection::~SocketConnection()
{
	socket.close();
}

std::string SocketConnection::getRemoteIp()
{
	return socket.remote_endpoint().address().to_string();
}

boost::asio::ip::tcp::socket& SocketConnection::getSocket()
{
	return socket;
}

void SocketConnection::read(std::function<void(std::vector <char>)> callback)
{
	read_callback = callback;
	readHeader();
}

void SocketConnection::readHeader()
{
	read_buffer = std::vector <char> (MESSAGE_HEADER_LENGTH, '\0');
	boost::asio::async_read(
		socket, 
		boost::asio::buffer(read_buffer, MESSAGE_HEADER_LENGTH), 
		std::bind(
			&SocketConnection::onReadHeader, 
			shared_from_this(), 
			std::placeholders::_1, 
			std::placeholders::_2
		)
	);
}

void SocketConnection::onReadHeader(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(!error)
	{
		int temp = 0;
		try
		{
			std::string str(read_buffer.begin(), read_buffer.end());
			temp = std::stoi(str);
			readBody();
		}
		catch(std::invalid_argument)
		{
			onMalformed();
		}
	}
	else
	{
		onError(error);
	}
}

void SocketConnection::readBody()
{
	int l = std::stoi(std::string(read_buffer.begin(), read_buffer.end()));
	std::vector <char> temp_read_buffer(l, '\0');
	boost::asio::async_read(
		socket, 
		boost::asio::buffer(temp_read_buffer, l), 
		std::bind(
			&SocketConnection::onReadBody, 
			shared_from_this(), 
			temp_read_buffer,
			std::placeholders::_1, 
			std::placeholders::_2
		)
	);
}

void SocketConnection::onReadBody(std::vector <char>& body_buffer, const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(!error)
	{
		if(body_buffer.size() != std::stoi(std::string(read_buffer.begin(), read_buffer.end())))
		{
			onMalformed();
		}
		else
		{
			read_buffer.insert(read_buffer.end(), body_buffer.begin(), body_buffer.end());
			malformed_messages = 0;
			onRead();
		}
	}
	else
	{
		onError(error);
	}
}

void SocketConnection::onRead()
{
	read_callback(read_buffer);
}

void SocketConnection::write(std::vector <char> text, std::function<void ()> callback)
{
	write_callback = callback;
	std::vector <char> to_write = text;
	boost::asio::async_write(
		socket,
		boost::asio::buffer(to_write), 
		std::bind(
			&SocketConnection::onWrite, 
			shared_from_this(), 
			std::placeholders::_1, 
			std::placeholders::_2
		)
	);
}

void SocketConnection::onWrite(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(!error)
	{
		write_callback();
	}
	else
	{
		onError(error);
	}
}

void SocketConnection::onMalformed()
{
	malformed_messages++;
	if(malformed_messages < CONNECTION_MAX_MALFORMED_IN_A_ROW)
	{
		readHeader();
	}
	else
	{
		throw ConnectionException("Too many malformed messages.");
	}
}

void SocketConnection::onError(const boost::system::error_code& error)
{
	throw ConnectionException(error.message());
}