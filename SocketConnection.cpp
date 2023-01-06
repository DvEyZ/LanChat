#include "SocketConnection.h"

SocketConnection::SocketConnection(boost::asio::io_context& iocontext)
	:socket(iocontext), header_buffer(), body_buffer(), error_callback()
{

}

SocketConnection::~SocketConnection()
{
    if(!socket.is_open())
    {
        socket.shutdown(boost::asio::socket_base::shutdown_both);
	    socket.close();
    }
}

std::string SocketConnection::getRemoteIp()
{
	return socket.remote_endpoint().address().to_string();
}

boost::asio::ip::tcp::socket& SocketConnection::getSocket()
{
	return socket;
}

void SocketConnection::setErrorCallback(std::function <void(ConnectionError)> callback)
{
	error_callback = callback;
}

void SocketConnection::read(std::function<void(MessageWrapper)> callback)
{
	read_callback = callback;
	readHeader();
}

void SocketConnection::close()
{
	socket.shutdown(boost::asio::socket_base::shutdown_both);
	socket.close();
}

void SocketConnection::readHeader()
{
	header_buffer = std::vector <char> (sizeof(uint32_t), '\0');
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
        read_wrapper.decodeHeader(header_buffer);
        readBody();
	}
	else
	{
		onError(ConnectionError(error));
	}
}

void SocketConnection::readBody()
{
	body_buffer = std::vector(read_wrapper.getLength(), '\0');
	boost::asio::async_read(
		socket, 
		boost::asio::buffer(body_buffer),
		[this] (const boost::system::error_code& error, std::size_t bytes_transferred)
		{
			onReadBody(error, bytes_transferred);
		}
	);
}

void SocketConnection::onReadBody(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	if(!error)
	{
		read_wrapper.decodeBody(body_buffer);
        onRead();
	}
	else
	{
		onError(ConnectionError(error));
	}
}

void SocketConnection::onRead()
{
	read_callback(read_wrapper);
}

void SocketConnection::write(MessageWrapper msg, std::function<void ()> callback)
{
	write_callback = callback;
	write_buffer = msg.encode();
	boost::asio::async_write(
		socket,
		boost::asio::buffer(write_buffer), 
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
		onError(ConnectionError(error));
	}
}

void SocketConnection::onError(ConnectionError error)
{
	this->error_callback(error);	// pass to error callback if unhandled.
}