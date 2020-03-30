#pragma once
#include <boost\asio.hpp>

using boost::asio::ip::tcp;

class User
{
public:
	User();
	User(boost::asio::io_service& ioService, tcp::socket& socket);
private:
	boost::asio::io_service* ioService_;
	tcp::socket* socket_;
};

