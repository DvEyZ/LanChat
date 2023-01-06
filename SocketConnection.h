#pragma once

#include "defines.h"
#include "Connection.h"
#include "ConnectionError.h"
#include "MessageWrapper.h"
#include <boost/asio.hpp>
#include <memory>

class ConnectionError;

class SocketConnection : public Connection, public std::enable_shared_from_this<SocketConnection>
{
public:
	SocketConnection(boost::asio::io_context& io_context);
	~SocketConnection();
	void read(std::function<void(MessageWrapper)> callback);
	void write(MessageWrapper m, std::function<void ()> callback);
	void setErrorCallback(std::function <void(ConnectionError)> callback);
	std::string getRemoteIp();
	void close();
	boost::asio::ip::tcp::socket& getSocket();
private:
	void readHeader();
	void onReadHeader(const boost::system::error_code& error, std::size_t bytes_transferred);
	void readBody();
	void onReadBody(const boost::system::error_code& error, std::size_t bytes_transferred);
	void onRead();
	
	void onWrite(const boost::system::error_code& error, std::size_t bytes_transferred);
	
	void onError(ConnectionError error);

	std::function <void(MessageWrapper)> read_callback;
	std::function <void(void)> write_callback;
	std::function <void(ConnectionError)> error_callback;

	boost::asio::ip::tcp::socket socket;

	std::vector <char> header_buffer;
	std::vector <char> body_buffer;
	std::vector <char> write_buffer;

    MessageWrapper read_wrapper;
    MessageWrapper write_wrapper;
};