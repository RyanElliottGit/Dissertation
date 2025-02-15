#include "pch.h"
#include "Card.h"
#include "Deck.h"
#include <ctime>
#include <iostream>
#include <string>
#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <boost\bind.hpp>
#include <chrono>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <Windows.h>
#include <map>

using boost::asio::ip::tcp;
using namespace std;
using namespace SuitNamespace;
using namespace FaceNamespace;

enum { maxLength = 1024 };

string port = "300";
string ip;
bool multiplayer = true;
Deck deck;

void cliReset() {
	system("cls");

	cout << "Welcome to Ryan's Blackjack!" << endl << endl;
}

class GetIp {
public:
	GetIp() {
		bool progress = false;
		while (!progress) {
			cliReset();
			cout << "Would you like to play (M)ultiplayer or (S)ingleplayer?" << endl;
			string ans;
			cin >> ans;
			if (ans == "M" || ans == "m") {
				cliReset();
				bool test = false;
				cout << "Enter the ip of the host: ";
				while (!test) {
					cin >> ip;
					boost::system::error_code error;
					boost::asio::ip::address::from_string(ip, error);
					if (!error) {
						test = true;
					}
					else { cout << "Enter a valid ip: "; }
				}
				cin.ignore();
				progress = true;
			}
			else if (ans == "S" || ans == "s") {
				multiplayer = false;
				progress = true;
			}
		}
	}
};

GetIp getIp;

//Some code adapted from https://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/example/echo/blocking_tcp_echo_client.cpp

boost::asio::io_service ioService;

tcp::resolver resolver(ioService);
tcp::resolver::query query(tcp::v4(), ip, port);
tcp::resolver::iterator tcpIterator = resolver.resolve(query);

tcp::socket s(ioService);

void writeServer(const char msg[], int length) {
	boost::asio::write(s, boost::asio::buffer(msg, length));
}

void readServer(char* reply, int length) {
	boost::asio::read(s, boost::asio::buffer(reply, length));
}


class Server
{
public:
	Server();
	int Initialize();
	int GetCard();
	void startDealer();
	int getDealerCards();

private:
	char data[2];
	char reply[maxLength];
};

Server::Server()
{
	if (multiplayer) {
		boost::asio::connect(s, tcpIterator);
	}
}

int Server::Initialize()
{
	cliReset();
	writeServer("3", 1);
	readServer(data, 1);
	cout << "Servers avalible are: ";
	for (int i = 1; i < atoi(data) + 1; i++) {
		cout << i << " ";
	}
	cout << endl;
	bool initialized = false;
	char lobbyNumber[1];
	cout << "Pick a room to join: ";
	while (!initialized) {
		cin >> lobbyNumber;
		data[0] = '2';
		data[1] = atoi(lobbyNumber);
		writeServer(data, 2);
		readServer(reply, 1);
		if (reply[0] == '1') {
			cout << "Game initialization successful." << endl;
			initialized = true;
		}
		cout << "Pick a valid room: ";
	}
	return 0;
}

int Server::GetCard()
{
	if (multiplayer) {
		data;
		data[0] = '1';
		writeServer(data, 1);
		readServer(reply, 2);
		return atoi(reply);
	}
	else {
		return deck.dealCard().face;
	}
}

void Server::startDealer() {
	bool run = false;
	writeServer("0", 1);
	readServer(reply, 1);
	if (reply[0] == '1') {
		cout << "Dealer cards inc." << endl;
		run = true;
	}
	while (!run) {
		writeServer("4", 1);
		readServer(reply, 1);
		if (reply[0] == '1') {
			cout << "Dealer cards inc." << endl;
			run = true;
		}
		else {
			Sleep(2000);
		}
	}
}

int Server::getDealerCards() {
	if (multiplayer) {
		readServer(reply, 2);
		return atoi(reply);
	}
	else {
		return deck.dealCard().face;
	}
}


int main()
{
	try
	{
		cout << "Trying to connect..." << endl;
		Server server = Server();
		if (multiplayer) {
			int i = server.Initialize();
		}

		cliReset();
		cin.ignore();
		cout << "Waiting for the next round to start...";

		deck.shuffle();
		bool gameContinue = true;
		bool badEntry = true;
		bool roundContinue = true;
		bool dealerRoundEnd = false;
		string yesNo;
		int handValue;
		int dealerHand[5];
		int dealerHandValue;
		int cardBuffer;

		while (gameContinue != false) {
			if (multiplayer) {
				server.startDealer();
			}
			cliReset();
			for (int i = 0; i < 5; i++) {
				dealerHand[i] = server.getDealerCards();
			}
			handValue = server.GetCard() + server.GetCard();
			cout << "The dealer shows a: " << dealerHand[0] << endl;
			if (handValue == 22) { handValue = 12; }
			if (handValue == 21) {
				cout << "You win with: 21, Blackjack!" << endl;
				roundContinue = false;
			}
			else {
				cout << "The value of your starting hand is: " << handValue << endl;
			}
			while (roundContinue == true) {
				cout << "Would You like to draw another card? (Y/N): ";
				cin >> yesNo;
				if (yesNo == "y" || yesNo == "Y") {
					cardBuffer = server.GetCard();
					if (cardBuffer == 11 && (handValue + cardBuffer) > 21) { cardBuffer = 1; }
					handValue += cardBuffer;
					if (handValue > 21) {
						cout << "You bust with: " << handValue << endl;
						roundContinue = false;
					}
					else if (handValue == 21) {
						cout << "You win with: 21" << endl;
						roundContinue = false;
					}
					else {
						cout << "Your hand is now: " << handValue << endl;
					}
				}
				else if (yesNo == "n" || yesNo == "N") {
					cout << "Your final hand was: " << handValue << endl;
					dealerHandValue = dealerHand[0] + dealerHand[1];
					cout << "The dealer has: " << dealerHandValue << endl;
					while (dealerRoundEnd != true) {
						if (dealerHandValue < handValue) {
							dealerHandValue += dealerHand[3];
							cout << "The dealer draws: " << dealerHand[3] << endl;
							if (dealerHandValue > 21) {
								cout << "The dealer draws to bust with: " << dealerHandValue << endl;
								cout << "You win with: " << handValue << endl << endl;
								dealerRoundEnd = true;
							}
							else if (dealerHandValue > handValue) {
								cout << "The dealer wins with: " << dealerHandValue << endl;
								dealerRoundEnd = true;
							}
							else {
								cout << "The dealer draws to a value of: " << dealerHandValue << endl;
							}
						}
						else {
							cout << "The dealer wins with: " << dealerHandValue << endl;
							dealerRoundEnd = true;
						}
					}
					handValue = 0;
					roundContinue = false;
				}
			}
			cout << "Would You like to play again? (Y/N): ";
			bool test = false;
			while (!test) {
				cin >> yesNo;
				if (yesNo == "y" || yesNo == "Y") {
					gameContinue = true;
					roundContinue = true;
					dealerRoundEnd = false;
					cliReset();
					cout << "Waiting for the next round to start...";
					test = true;
				}
				else if (yesNo == "n" || yesNo == "N") {
					gameContinue = false;
					test = true;
				}
				else {
					cout << "Enter Y/N" << endl;
				}
			}
			if (deck.deckSize() < 10) {
				deck = Deck();
				deck.shuffle();
			}
		}
	}
	catch (exception& e)
	{
		cerr << "Exception: " << e.what() << "\n";
		cin.ignore();
		getIp = GetIp();
		main();
	}

	cin.ignore();
	return 0;
}

