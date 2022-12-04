#pragma once

#include "Logger.h"
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

class Connection
{
public:
	Connection(boost::asio::io_context& io_context);
	~Connection();
	void read(std::function<void(std::vector <char>)> callback);
	void write(std::vector <char> text, std::function<void> callback);
	boost::asio::ip::tcp::socket& getSocket();
private:
	void readHeader();
	void onReadHeader(const boost::system::error_code& error, std::size_t bytes_transferred);
	void readBody();
	void onReadBody(const boost::system::error_code& error, std::size_t bytes_transferred);
	void onRead();
	
	std::function <void(std::vector <char>)> read_callback;
	std::function <void()> write_callback;

	boost::asio::ip::tcp::socket socket;
	boost::system::error_code error_status;

	std::vector <char> read_buffer;
	
	Logger* logger;
};