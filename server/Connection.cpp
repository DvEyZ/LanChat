//TODO:
//Implement identify(), onReadIdentificationBody(), writeIdentificationResponse(), onWriteIdentificationResponse()

#include "Connection.h"

Connection::Connection(boost::asio::io_context& io_context_, Chat* chat_)
    :sock(io_context_), chat(chat_)
{}

Connection::~Connection()
{
    sock.close(error_status);
}

boost::asio::ip::tcp::socket& Connection::socket()
{
    return sock;
}

void Connection::run()
{
    server->awaiting_for_identification.insert(shared_from_this());
    if(identify())
    {
        Chat.join(boost::shared_from_this());
    }
    server->awaiting_for_identification.erase(shared_from_this());
}

void Connection::postMessage(ChatMessage message)
{
    bool idle = recentMsgWrite.empty();
    recentMsgWrite.push(message);
    if(idle)
    {
        write();
    }
}

bool identify()
{
    readIdentificationHeader();
}

void readIdentificationHeader()
{
    boost::asio::async_read(sock, boost::asio::buffer(recentMsgHeaderBuffer), boost::bind(&Connection::onReadIdentificationHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void onReadIdentificationHeader(const boost::system::error_code& error, std::size_t bytes_transferred);
{
    if(!error)
    {
        identifyMessageTemp = new IdentifyMessage();
        if(identifyMessageTemp.decodeHeader(recentMsgHeaderBuffer))
        {
            recentMsgBodyBuffer = new char[recentMsgRead.getBodyLength()];
            readIdentificationBody();
        }
    }
    else
    {
        onError(error);
    }
}   // Todo: Find a way to return value on failure

void readIdentificationBody()
{
    boost::asio::async_read(sock, boost::asio::buffer(recentMsgBodyBuffer), boost::bind(&Connection::onReadIdentificationBody, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void onReadIdentificationBody(const boost::system::error_code& error, std::size_t bytes_transferred);
{
}

void writeIdentificationResponse()
{

}

void onWriteIdentificationResponse(const boost::system::error_code& error, std::size_t bytes_transferred)
{

}


void Connection::readHeader()
{
    boost::asio::async_read(sock, boost::asio::buffer(recentMsgHeaderBuffer), boost::bind(&Connection::onReadHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::readBody()
{
    boost::asio::async_read(sock, boost::asio::buffer(recentMsgBodyBuffer), boost::bind(&Connection::onReadBody, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::write()
{
    boost::asio::async_write(sock, boost::asio::buffer(recentMsgWrite.front().encodeMessage()), boost::bind(&Connection::onWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::onReadHeader(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        if(recentMsgRead.decodeHeader(recentMsgHeaderBuffer))
        {
            recentMsgBodyBuffer = new char[recentMsgRead.getBodyLength()];
            readBody();
        }
        else
        {
            chat.log("Bad message header from" + sock.remote_endpoint());
            readHeader();
        }
    }
    else
    {
        onError(error);
    }

}

void Connection::onReadBody(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        if(recentMsgRead.decodeBody(recentMsgBodyBuffer))
        {
            chat.messageIncoming(recentMsgRead);
        }
        else
        {
            chat.log("Bad message body from" + sock.remote_endpoint());
        }
        recentMsgHeaderBuffer = "\0\0\0\0\0\0\0\0"
        delete[] RecentMsgBodyBuffer;
        readHeader();
    }
    else
    {
        onError(error);
    }
}

void Connection::onWrite(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        recentMsgWrite.pop();
        if(!recentMsgWrite.empty())
        {
            write();
        }
    }
    else
    {
        onError(error);
    }
}

void Connection::onError(const boost::system::error_code& error)
{
    error_status = error;
    chat.log(error.message());
    chat.leave(shared_from_this());
}

std::string Connection::getUser()
{
    return user;
}