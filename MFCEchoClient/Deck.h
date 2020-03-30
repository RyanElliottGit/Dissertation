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
	void shuffle();
	void dealCard(Player player);

private:
	int deckSize();
	vector<Card> deck;

};
