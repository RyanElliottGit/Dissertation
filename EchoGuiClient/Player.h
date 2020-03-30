#pragma once
#include "Card.h"
#include <iostream>
#include <vector>

using namespace std;

class Player
{
public:
	Player();
private:
	vector<Card> hand();
};

