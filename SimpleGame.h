#pragma once
#include "Game.h"
class SimpleGame :	public Game
{
public:
	SimpleGame(int size) : Game(size) {}

	void UpdateScore(int currrentPlayer);
};

