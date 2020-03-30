#include "stdafx.h"
#include "pch.h"
#include "Card.h"

Card::Card()
{
}

Card::Card(Suit suit, Face face)
{
	face = this->face;
	suit = this->suit;
}
