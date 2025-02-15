// BoostTestServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include "pch.h"
#include "Card.h"
#include "Deck.h"
#include "User.h"
#include <ctime>
#include <iostream>
#include <string>
#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <boost\bind.hpp>
#include <chrono>
#include <iomanip>

using boost::asio::ip::tcp;
using namespace std;
using namespace SuitNamespace;
using namespace FaceNamespace;

class Room {
public:
	Room(int num) {
		roomNumber = num;
	}
	Deck deck = Deck();
	int dealerHand[5];
	int roomNumber;
	int readyUsers = 0;
	int inGameUsers = 0;
	int users = 0;
	bool isFull() {
		if (users >= maxUsers) {
			return true;
		}
		else return false;
	}
	void dealerDraw() {
		for (int i = 0; i < 5; i++) {
			dealerHand[i] = deck.dealCard().face;
		}
	}
	bool readyCheck() {
		if (readyUsers == users) {
			inGameUsers++;
			if (inGameUsers == 1) {
				dealerDraw();
			}
			if (inGameUsers == readyUsers) {
				readyUsers = 0;
				inGameUsers = 0;
			}
			return true;
		}
		return false;
	}
private:
	int maxUsers = 5;
};

vector<Room> rooms;
int numOfRooms;
int errorDel = 0;

//Some code addapted from https://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/example/echo/async_tcp_echo_server.cpp
class Session
{
public:
	Session(boost::asio::io_service& ioService)
		: socket(ioService)
	{
	}

	tcp::socket& socketAlias()
	{
		return socket;
	}

	void start()
	{
		socket.async_read_some(boost::asio::buffer(dataArray, maxLength),
			boost::bind(&Session::handleRead, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

private:
	void handleRead(const boost::system::error_code& error,
		size_t bytesTransferred)
	{
		if (!error)
		{
			char dataChar[1];
			dataChar[0] = dataArray[0];
			int dataInt = atoi(dataChar);
			int roomInt;
			if (dataInt == 2) {
				roomInt = dataArray[1];
			}
			char cardValue[2];
			switch (dataInt)
			{
			case 0: cout << "Player Ready in Room " << room->roomNumber << endl; //Try to start round 
				room->readyUsers += 1;
				if (room->readyCheck()) {
					cout << "Room ready" << endl;
					dataWrite("1", 1);
					for (int card : room->dealerHand) {
						_itoa(card, cardValue, 10); //Convert int into string
						dataWrite(cardValue, 2);
					}
				}
				else {
					dataWrite("0", 1);
				}
				break;
			case 1: cout << "Player Card Draw" << endl; //Draw Card and send to client
				if (room->deck.deckSize() < 10) {
					room->deck = Deck();
					room->deck.shuffle();
				}
				_itoa(room->deck.dealCard().face, cardValue, 10); //Convert int into string
				dataWrite(cardValue, 2);
				break;
			case 2: cout << "Picking room" << endl; //Pick room
				if (roomInt > numOfRooms) {
					cout << "Room doesn't exist" << endl;
					dataWrite("0", 1);
				}
				else if (rooms[roomInt-1].isFull()) {
					cout << "Room Full" << endl;
					dataWrite("0", 1);
				}
				else {
					room = &rooms[roomInt-1];
					dataWrite("1", 1);
					room->users += 1;
					if (room->users == 1) {
						room->deck = Deck();
						room->deck.shuffle();
					}
					cout << "User joined Room " << room->roomNumber << ", " << room->users << " users present" << endl;
				}
				break;
			case 3: cout << "Listing rooms" << endl; //List rooms
				char s[3];
				_itoa(numOfRooms, s, 10);
				dataWrite(s, 1);
				break;
			case 4: cout << "Ready check in room " << room->roomNumber << endl;
				if (room->readyCheck()) {
					dataWrite("1", 1);
					for (int card : room->dealerHand) {
						_itoa(card, cardValue, 10); //Convert int into string
						dataWrite(cardValue, 2);
					}
				}
				else {
					dataWrite("0", 1);
				}
				break;
			default:
				break;
			}
		}
		else
		{
			if (errorDel < 1) {
				errorDel++;
				closeConnection();
				delete this;
			}
		}
	}

	void dataWrite(const char data[], int dataLength) {
		boost::asio::async_write(socket, boost::asio::buffer(data, dataLength),
			boost::bind(&Session::handleWrite, this, boost::asio::placeholders::error));
	}

	void handleWrite(const boost::system::error_code& error)
	{
		if (!error)
		{
			socket.async_read_some(boost::asio::buffer(dataArray, maxLength),
				boost::bind(&Session::handleRead, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			closeConnection();
			delete this;
		}
	}

	void closeConnection() {
		if (room != NULL) {
			room->users -= 1;
		}
		cout << "User disconnected" << endl;
	}

	tcp::socket socket;
	enum { maxLength = 1024 };
	char dataArray[maxLength];
	Room* room;
};


class Server
{
public:
	Server(boost::asio::io_service& ioService, short port)
		: ioService_(ioService),
		acceptor_(ioService, tcp::endpoint(tcp::v4(), port))
	{
		startAccept();
	}

private:
	void startAccept()
	{
		Session* newSession = new Session(ioService_);
		acceptor_.async_accept(newSession->socketAlias(),
			boost::bind(&Server::handleAccept, this, newSession,
				boost::asio::placeholders::error));
	}

	void handleAccept(Session* newSession,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			newSession->start();
		}
		else
		{
			delete newSession;
		}

		startAccept();
	}

	boost::asio::io_service& ioService_;
	tcp::acceptor acceptor_;
};

int main()
{
	char c[1];
	cout << "How many rooms would you like to create(1-9)" << endl;
	cin >> c;
	numOfRooms = atoi(c);
	while (!(numOfRooms > 0 && numOfRooms < 10)) {
		cout << "Please enter a valid number" << endl;
		cin >> c;
		numOfRooms = atoi(c);
	}
	for (int i = 0; i < numOfRooms; i++) {
		Room room = Room(i+1);
		rooms.push_back(room);
	}
	try
	{
		const char* port = "300";

		boost::asio::io_service ioService;

		Server server(ioService, atoi(port));

		ioService.run();
	}
	catch (exception& e)
	{
		cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}