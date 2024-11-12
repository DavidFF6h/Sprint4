#include "SimpleGame.h"

void SimpleGame::UpdateScore(int currrentPlayer)
{
    if (checkForSOS(boardSize)) {
		gameOver = true;
		switch (currrentPlayer)
		{
		case 0:
			SetWinner(0);
			break;
		case 1:
			SetWinner(1);
			break;
		default:
			printf("Error in player switching\n");
			break;
		}
    }
	else if (isFull()) {
		SetWinner(2);
		gameOver = true;	
	}
}
