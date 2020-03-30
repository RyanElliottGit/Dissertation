#pragma once
#include <map>
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

//Some code adapted from https://stackoverflow.com/questions/261963/how-can-i-iterate-over-an-enum

namespace SuitNamespace {
	enum Suit { Hearts, Diamonds, Clubs, Spades };

	static const Suit SuitAll[] = { Hearts, Diamonds, Clubs, Spades };

}

namespace FaceNamespace {
	enum Face {
		Ace = 11, Two = 2, Three = 3, Four = 4, Five = 5, Six = 6,
		Seven = 7, Eight = 8, Nine = 9, Ten = 10, Jack = 10, Queen = 10, King = 10
	};

	static const Face FaceAll[] = { Ace, Two, Three, Four, Five, Six,
		Seven, Eight, Nine, Ten, Jack, Queen, King };
}

using namespace std;
using namespace SuitNamespace;
using namespace FaceNamespace;

static map<Suit, const char *> name = {
	{Hearts, "Hearts"}, {Diamonds, "Diamonds"}, {Clubs, "Clubs"}, {Spades, "Spades"}
};

class Card
{
public:
	Card();
	Card(Suit, Face);
	Face Value();

	//private:
	Face face;
	Suit suit;
};

