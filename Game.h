#ifndef GAME_H
#define GAME_H

#include <vector>
#include "CellState.h"
#include <set>
#include <tuple>
#include <cstdlib>
#include <ctime> 


class Game {
public:
    Game(int size);

    void initGameBoard(int size);
    void resetGame(int size);
    bool setCellState(int row, int col, int currentPlayer, int selected_radio1, int selected_radio2);
    bool checkForSOS(int size);
    CellState getCurrentPlayerSymbol(int currentPlayer, int selected_radio1, int selected_radio2);
    const std::vector<std::vector<CellState>>& getGameBoard() const;

    int GetBlueScore() { return blueScore; }

    int GetRedScore() { return redScore; }

    void TryCPUMove(int& currentPlayer, int bluePlayerType, int redPlayerType);

    void findBestMove(int size, int currentPlayer);

    virtual void UpdateScore(int currentPlayer) = 0;

    bool gameOver = false;

    int GetWinner() { return winner; }
    void SetWinner(int win) { winner = win; }

    std::vector<std::vector<CellState>> gameBoard;
protected:
    int boardSize;
    int blueScore = 0;
    int redScore = 0;
    int numSOS = 0;
    bool isFull();

private:
    std::set<std::tuple<int, int, int, int, int, int>> sosPatterns;
    int winner;
};

#endif


