#include "Connection.h"

Connection::Connection(boost::asio::io_context& iocontext)
	:socket(iocontext)
{

}
