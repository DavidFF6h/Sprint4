#pragma once
#include "Game.h"

class GeneralGame :	public Game
{
public:
	GeneralGame(int size) : Game(size) {}

	void UpdateScore(int currrentPlayer);


};

