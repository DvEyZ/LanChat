#include "SocketConnection.h"

SocketConnection::SocketConnection(boost::asio::io_context& iocontext)
	:socket(iocontext), malformed_messages(0), header_buffer(), body_buffer()
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
	header_buffer = std::vector <char> (MESSAGE_HEADER_LENGTH, '\0');
	boost::asio::async_read(
		socket, 
		boost::asio::buffer(header_buffer), 
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
			std::string str(header_buffer.begin(), header_buffer.end());
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
	int l = std::stoi(std::string(header_buffer.begin(), header_buffer.end()));
	body_buffer = std::vector(l, '\0');
	boost::asio::async_read(
		socket, 
		boost::asio::buffer(body_buffer),
		[this, l] (const boost::system::error_code& error, std::size_t bytes_transferred)
		{
			onReadBody(l, error, bytes_transferred);
		}
	);
}

void SocketConnection::onReadBody(int l, const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(!error)
	{
		if(body_buffer.size() != l)	// tu jest błąd
		{
			onMalformed();
		} 
		else
		{
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
	std::vector <char> return_buffer;
	return_buffer.insert(return_buffer.end(), header_buffer.begin(), header_buffer.end());
	return_buffer.insert(return_buffer.end(), header_buffer.begin(), header_buffer.end());
	read_callback(return_buffer);
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