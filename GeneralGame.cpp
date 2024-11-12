#include "GeneralGame.h"

void GeneralGame::UpdateScore(int currrentPlayer)
{

    if (checkForSOS(boardSize)) {
        switch (currrentPlayer)
        {
        case 0:
            blueScore+=numSOS;
            break;
        case 1:
            redScore += numSOS;
            break;
        default:
            printf("Error in player switching\n");
            break;
        }
    }

    if (isFull()) {
        gameOver = true;
        if (blueScore > redScore) {
            SetWinner(0);
        }
        else if (blueScore < redScore) {
            SetWinner(1);
        }
        else {
            SetWinner(2);
        }
    }
}


