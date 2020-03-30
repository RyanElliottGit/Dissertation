#pragma once
#include "Card.h"
#include <vector>
#include <iostream>

using namespace std;

class Deck
{
public:
	Deck();
	void shuffle();
	Card dealCard();
	void outDeck();
	int deckSize();

private:
	vector<Card> deck;

};
