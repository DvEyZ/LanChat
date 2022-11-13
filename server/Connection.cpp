//FINISHEDD

#include "Connection.h"

Connection::Connection(boost::asio::io_context& _io_context, Chat* _chat)
    :sock(_io_context), chat(_chat)
{

}

Connection::~Connection()
{
    sock.close(error_status);
}

boost::asio::ip::tcp::socket& Connection::socket()
{
    return sock;
}

std::string Connection::getUser()
{
    return user;
}

void Connection::run()
{
    server->awaiting_for_identification.insert(shared_from_this());
    recentReadBuffer = std::vector<char>(8,'\0');
    readIdentificationHeader();
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

bool Connection::identify()
{
    IdentifyResponseMessage::Status status;
    status = chat->auth->authenticate(*identifyMessageTemp);
    if(status == IdentifyResponseMessage::Status::ok)
        status = chat->auth->permitConnection(shared_from_this());
    
    identifyResponseMessageTemp = new IdentifyResponseMessage(status);

    writeIdentificationResponse();
}

void Connection::identificationFailure(IdentifyResponseMessage::Status status)
{
    identifyResponseMessageTemp = new IdentifyResponseMessage(status);
    writeIdentificationResponse();
}

void Connection::readIdentificationHeader()
{
    boost::asio::async_read(sock, boost::asio::buffer(recentReadBuffer), boost::bind(&Connection::onReadIdentificationHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::onReadIdentificationHeader(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        identifyMessageTemp = new IdentifyMessage();
        if(identifyMessageTemp->decodeHeader(recentReadBuffer))
        {
            recentReadBuffer = std::vector<char>(identifyMessageTemp->getLength(),'\0');
            readIdentificationBody();
        }
        else
        {
            identificationFailure(IdentifyResponseMessage::Status::auth_failed_malformed);
        }
    }
    else
    {
        onError(error);
    }
}

void Connection::readIdentificationBody()
{
    boost::asio::async_read(sock, boost::asio::buffer(recentReadBuffer), boost::bind(&Connection::onReadIdentificationBody, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::onReadIdentificationBody(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        if(identifyMessageTemp->decodeBody(recentReadBuffer))
        {
            identify();
        }
        else
        {
            identificationFailure(IdentifyResponseMessage::Status::auth_failed_malformed);
        }
    }
    else
    {
        onError(error);
    }
}

void Connection::writeIdentificationResponse()
{
    boost::asio::async_write(sock, boost::asio::buffer(identifyResponseMessageTemp->encodeMessage()), boost::bind(&Connection::onWriteIdentificationResponse, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::onWriteIdentificationResponse(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        if(identifyResponseMessageTemp->getStatus() == IdentifyResponseMessage::Status::ok)
        {
            chat->join(shared_from_this());
            recentReadBuffer = std::vector<char>(8,'\0');
            readHeader();
        }
        server->awaiting_for_identification.erase(shared_from_this());
    }
    else
    {
        onError(error);
    }
}


void Connection::readHeader()
{
    boost::asio::async_read(sock, boost::asio::buffer(recentReadBuffer), boost::bind(&Connection::onReadHeader, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::readBody()
{
    boost::asio::async_read(sock, boost::asio::buffer(recentReadBuffer), boost::bind(&Connection::onReadBody, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::write()
{
    boost::asio::async_write(sock, boost::asio::buffer(recentMsgWrite.front().encodeMessage()), boost::bind(&Connection::onWrite, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::onReadHeader(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error)
    {
        if(recentMsgRead.decodeHeader(recentReadBuffer))
        {
            recentReadBuffer = std::vector<char>(recentMsgRead.getBodyLength(), '\0');
            readBody();
        }
        else
        {
            for(auto i : chat->loggers)
                i->log("Bad message header from" + sock.remote_endpoint().address().to_string());
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
        if(recentMsgRead.decodeBody(recentReadBuffer))
        {
            chat->messageIncoming(recentMsgRead);
        }
        else
        {
            for(auto i : chat->loggers)
                i->log("Bad message body from" + sock.remote_endpoint().address().to_string());
        }
        recentReadBuffer = std::vector<char>(8, '\0');
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
    for(auto i : chat->loggers)
        i->log(error.message());
    chat->leave(shared_from_this());
}