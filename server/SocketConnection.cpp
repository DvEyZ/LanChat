#include "SocketConnection.h"

SocketConnection::SocketConnection(boost::asio::io_context& iocontext)
	:socket(iocontext)
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

boost::asio::ip::tcp::socket& getSocket()
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
	read_buffer = std::vector(MESSAGE_HEADER_LENGTH, '\0');
	boost::asio::async_read(
		socket, 
		boost::asio::buffer(read_buffer), 
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
			readHeader();
		}
	}
	else
	{
		onError(error);
	}
}

void SocketConnection::readBody()
{
	std::vector <char> temp_read_buffer(std::stoi(std::string(read_buffer.begin(), read_buffer.end())), '\0');
	boost::asio::async_read(
		socket, 
		boost::asio::buffer(temp_read_buffer), 
		std::bind(
			&SocketConnection::onReadBody, 
			shared_from_this(), 
			temp_read_buffer,
			std::placeholders::_1, 
			std::placeholders::_2
		)
	);
}

void SocketConnection::onReadBody(std::vector <char> body_buffer, const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(!error)
	{
		if(body_buffer.size() != std::stoi(std::string(read_buffer.begin(), read_buffer.end())))
		{
			onMalformed();
			readHeader();
		}
		else
		{
			read_buffer.insert(read_buffer.end(), body_buffer.begin(), body_buffer.end());
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
	
}

void SocketConnection::onError(const boost::system::error_code& error)
{
	throw new ConnectionException(error);
}