#pragma once
#include "Card.h"
#include <vector>
#include <iostream>
#include "Player.h"

using namespace std;

class Deck
{
public:
	Deck();
	Deck(int decks);
	void shuffle();
	Card dealCard();
	void outDeck();
	int deckSize();

private:
	vector<Card> deck;

};
