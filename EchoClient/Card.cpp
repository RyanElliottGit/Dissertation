#include "pch.h"
#include "Card.h"

Card::Card()
{
}

Card::Card(Suit suit, Face face)
{
	this->face = face;
	this->suit = suit;
}

Face Card::Value()
{
	return Card::face;
}
