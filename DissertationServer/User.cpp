#include "pch.h"
#include "User.h"

User::User()
{
}

User::User(boost::asio::io_service& ioService, tcp::socket& socket)
{
	ioService_ = &ioService;
	socket_ = &socket;
}
