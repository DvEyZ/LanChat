#pragma once

#include "../defines.h"
#include "Connection.h"
#include "SocketConnectionError.h"
#include <boost/asio.hpp>
#include <memory>

class SocketConnectionError;

class SocketConnection : public Connection, public std::enable_shared_from_this<SocketConnection>
{
public:
	SocketConnection(boost::asio::io_context& io_context);
	~SocketConnection();
	void read(std::function<void(std::vector <char>)> callback);
	void write(std::vector <char> text, std::function<void ()> callback);
	void setErrorCallback(std::function <void(SocketConnectionError)> callback);
	std::string getRemoteIp();
	boost::asio::ip::tcp::socket& getSocket();
private:
	void readHeader();
	void onReadHeader(const boost::system::error_code& error, std::size_t bytes_transferred);
	void readBody();
	void onReadBody(int l, const boost::system::error_code& error, std::size_t bytes_transferred);
	void onRead();
	
	void onWrite(const boost::system::error_code& error, std::size_t bytes_transferred);
	
	void onMalformed();
	int malformed_messages;

	void onError(SocketConnectionError error);

	std::function <void(std::vector <char>)> read_callback;
	std::function <void(void)> write_callback;
	std::function <void(SocketConnectionError)> error_callback;

	boost::asio::ip::tcp::socket socket;

	std::vector <char> header_buffer;
	std::vector <char> body_buffer;
};