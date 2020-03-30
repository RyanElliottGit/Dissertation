#include "stdafx.h"
#include "pch.h"
#include "Deck.h"
#include <algorithm>

//Maybe add double/tripple deck

Deck::Deck()
{
	for (Suit suit : SuitAll) {
		for (Face face : FaceAll) {
			//			cout << name[suit] << ", " << face << endl;
			deck.push_back(Card(suit, face));
		}
	}
}

void Deck::shuffle()
{
	random_shuffle(deck.begin(), deck.end());
}

void Deck::dealCard(Player player)
{
	Card newCard = deck[0];
	deck.pop_back();
	//Need to figure when to shuffle the deck
}
int Deck::deckSize()
{
	return 0;
}