#include "pch.h"
#include "Deck.h"
#include <algorithm>
#include <cstdlib>

//Maybe add double/tripple deck

Deck::Deck()
{
	for (Suit suit : SuitAll) {
		for (Face face : FaceAll) {
			//cout << name[suit] << ", " << face << endl;
			Card card = Card(suit, face);
			deck.push_back(card);
			//deck.at(0).face = card.face;
			//cout << deck.at(0).face << " " << face << endl;
		}
	}
}

void Deck::shuffle()
{
	srand(time(0));
	int i = rand() % 10 + 1;
	while (i > 0) {
		random_shuffle(deck.begin(), deck.end());
		i--;
	};
}


Card Deck::dealCard()
{
	Card newCard = deck.at(deck.size() - 1);
	deck.pop_back();
	return newCard;
	//Need to figure when to shuffle the deck
}

void Deck::outDeck()
{
	for (vector<Card>::iterator it = deck.begin(); it != deck.end(); ++it) {
		Card card = *it;
		cout << card.face << endl;
	}
}

int Deck::deckSize()
{
	return deck.size();
}