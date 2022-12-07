#pragma once

#include "Connection.h"
#include "ConnectionException.h"
#include <boost/asio.hpp>
#include <memory>

class SocketConnection : public Connection, public std::enable_shared_from_this<SocketConnection>
{
public:
	SocketConnection(boost::asio::io_context& io_context);
	~SocketConnection();
	void read(std::function<void(std::vector <char>)> callback);
	void write(std::vector <char> text, std::function<void ()> callback);
	std::string getRemoteIp();
	boost::asio::ip::tcp::socket& getSocket();
private:
	void readHeader();
	void onReadHeader(const boost::system::error_code& error, std::size_t bytes_transferred);
	void readBody();
	void onReadBody(std::vector <char> body_buffer, const boost::system::error_code& error, std::size_t bytes_transferred);
	void onRead();
	
	void onWrite(const boost::system::error_code& error, std::size_t bytes_transferred);
	
	void onMalformed();
	void onError(const boost::system::error_code& error);

	std::function <void(std::vector <char>)> read_callback;
	std::function <void()> write_callback;

	boost::asio::ip::tcp::socket socket;

	std::vector <char> read_buffer;
	
	Logger* logger;
};