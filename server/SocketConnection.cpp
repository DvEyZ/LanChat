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
		boost::asio::buffer(read_buffer), 
		[this] (const boost::system::error_code& error, std::size_t bytes_transferred)
		{
				onReadHeader(error, bytes_transferred);
		}
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
		boost::asio::buffer(temp_read_buffer), 
		[this, &temp_read_buffer] (const boost::system::error_code& error, std::size_t bytes_transferred)
		{
			onReadBody(temp_read_buffer, error, bytes_transferred);
		}
	);
}

void SocketConnection::onReadBody(std::vector <char>& body_buffer, const boost::system::error_code& error, std::size_t bytes_transferred)
{
	std::cout << "onReadBody() out";

	if(!error)
	{
		if(body_buffer.size() != std::stoi(std::string(read_buffer.begin(), read_buffer.end())))	// tu jest błąd
		{
			onMalformed();
		}
		else
		{
			read_buffer.insert(read_buffer.end(), body_buffer.begin(), body_buffer.end());
			malformed_messages = 0;
			std::cout << "onReadBody() in";
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
	std::cout << "onRead()";
	read_callback(read_buffer);
}

void SocketConnection::write(std::vector <char> text, std::function<void ()> callback)
{
	write_callback = callback;
	std::vector <char> to_write = text;
	boost::asio::async_write(
		socket,
		boost::asio::buffer(to_write), 
		[this] (const boost::system::error_code& error, std::size_t bytes_transferred)
		{
			onWrite(error, bytes_transferred);
		}
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