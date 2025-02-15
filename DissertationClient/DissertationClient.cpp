//// BoostTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//
//#include "pch.h"
//#include <iostream>
//#include <boost/array.hpp>
//#include <boost/asio.hpp>
//#include <string>
//
//using boost::asio::ip::tcp;
//
//int main(int argc, char* argv[])
//{
//	std::string clientAddress = "127.0.0.1";
//	//std::cout << "Enter the address to connect to:";
//	//std::cin >> clientAddress;
//	std::string msg = "Hello";
//
//	//std::cout << "Echo message:";
//	//std::cin >> msg;//take msg input
//	try
//	{
//		boost::asio::io_service io_service;
//
//		tcp::resolver resolver(io_service);
//		tcp::resolver::query query(clientAddress, "daytime");
//		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
//
//		tcp::socket socket(io_service);
//		boost::asio::connect(socket, endpoint_iterator);
//
//		for (;;)
//		{
//			boost::array<char, 128> buf;
//			boost::system::error_code error;
//			
//			boost::asio::write(socket, boost::asio::buffer(msg));//write a msg 
//
//			size_t reply = socket.read_some(boost::asio::buffer(buf), error);//read reply
//
//			if (error == boost::asio::error::eof)
//				continue; // Connection closed cleanly by peer.
//			else if (error)
//				continue;
//				//throw boost::system::system_error(error); // Some other error.
//
//			std::cout.write(buf.data(), reply);
//		}
//	}
//	catch (std::exception& e)
//	{
//		std::cerr << e.what() << std::endl;
//	}
//	system("pause");
//	return 0;
//}

#include "pch.h"
#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <chrono>
#include <iomanip>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
	try
	{
		//if (argc != 2)
		//{
		//	std::cerr << "Usage: client <host>" << std::endl;
		//	return 1;
		//}

		std::string ip = "127.0.0.1";

		boost::asio::io_context io_context;

		tcp::resolver resolver(io_context);
		tcp::resolver::results_type endpoints =
			resolver.resolve(ip, "daytime");

		tcp::socket socket(io_context);
		boost::asio::connect(socket, endpoints);

		for (;;)
		{
			boost::array<char, 128> buf;
			boost::system::error_code error;

			size_t len = socket.read_some(boost::asio::buffer(buf), error);

			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			std::cout.write(buf.data(), len);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}