#include "Game.h"

Game::Game(int size) {
    boardSize = size;
    initGameBoard(boardSize);
}

void Game::initGameBoard(int size) {
    gameOver = false;
    gameBoard.clear();
    sosPatterns.clear();
    redScore = 0;
    blueScore = 0;
    gameBoard.resize(boardSize, std::vector<CellState>(boardSize, EMPTY));
}

void Game::resetGame(int size) {
    boardSize = size;
    initGameBoard(size);
}

CellState Game::getCurrentPlayerSymbol(int currentPlayer, int selected_radio1, int selected_radio2) {
    if (currentPlayer == 0) { // Blue player
        return selected_radio1 == 0 ? S_BLUE : O_BLUE; // S or O
    }
    else { // Red player
        return selected_radio2 == 0 ? S_RED : O_RED; // S or O
    }
}

bool Game::setCellState(int row, int col, int currentPlayer, int selected_radio1, int selected_radio2) {
    if (gameOver) return false;
    if (gameBoard[row][col] == EMPTY) {
        gameBoard[row][col] = getCurrentPlayerSymbol(currentPlayer, selected_radio1, selected_radio2);
        UpdateScore(currentPlayer);
        return true;
    }
    else { return false; }
}

bool Game::checkForSOS(int size) {
    bool foundSOS = false;
    numSOS = 0;

    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            // Check horizontal SOS
            if (col + 2 < size) {  // Ensure we have enough columns for horizontal SOS
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    (gameBoard[row][col + 1] == O_BLUE || gameBoard[row][col + 1] == O_RED) &&
                    (gameBoard[row][col + 2] == S_BLUE || gameBoard[row][col + 2] == S_RED)) {

                    auto pattern = std::make_tuple(row, col, row, col + 1, row, col + 2);
                    if (sosPatterns.find(pattern) == sosPatterns.end()) {
                        numSOS++;
                        sosPatterns.insert(pattern);
                        foundSOS = true;
                    }
                }
            }

            // Check vertical SOS
            if (row + 2 < size) {  // Ensure we have enough rows for vertical SOS
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    (gameBoard[row + 1][col] == O_BLUE || gameBoard[row + 1][col] == O_RED) &&
                    (gameBoard[row + 2][col] == S_BLUE || gameBoard[row + 2][col] == S_RED)) {

                    auto pattern = std::make_tuple(row, col, row + 1, col, row + 2, col);
                    if (sosPatterns.find(pattern) == sosPatterns.end()) {
                        numSOS++;
                        sosPatterns.insert(pattern);
                        foundSOS = true;
                    }
                }
            }

            // Check diagonal SOS (top-left to bottom-right)
            if (row + 2 < size && col + 2 < size) {  // Ensure space for diagonal from top-left to bottom-right
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    (gameBoard[row + 1][col + 1] == O_BLUE || gameBoard[row + 1][col + 1] == O_RED) &&
                    (gameBoard[row + 2][col + 2] == S_BLUE || gameBoard[row + 2][col + 2] == S_RED)) {

                    auto pattern = std::make_tuple(row, col, row + 1, col + 1, row + 2, col + 2);
                    if (sosPatterns.find(pattern) == sosPatterns.end()) {
                        sosPatterns.insert(pattern);
                        numSOS++;
                        foundSOS = true;
                    }
                }
            }

            // Check diagonal SOS (bottom-left to top-right)
            if (row - 2 >= 0 && col + 2 < size) {  // Ensure space for diagonal from bottom-left to top-right
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    (gameBoard[row - 1][col + 1] == O_BLUE || gameBoard[row - 1][col + 1] == O_RED) &&
                    (gameBoard[row - 2][col + 2] == S_BLUE || gameBoard[row - 2][col + 2] == S_RED)) {

                    auto pattern = std::make_tuple(row, col, row - 1, col + 1, row - 2, col + 2);
                    if (sosPatterns.find(pattern) == sosPatterns.end()) {
                        sosPatterns.insert(pattern);
                        numSOS++;
                        foundSOS = true;
                    }
                }
            }
        }
    }

    return foundSOS;
}

bool Game::isFull() {
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (gameBoard[i][j] == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

const std::vector<std::vector<CellState>>& Game::getGameBoard() const {
    return gameBoard;
}

void Game::TryCPUMove(int& currentPlayer, int bluePlayerType, int redPlayerType)
{
    if (gameOver) return;
    if(currentPlayer == 0 && bluePlayerType == 1) {
        findBestMove(boardSize, currentPlayer);
        currentPlayer = 1 - currentPlayer;
        return;
    }
    else if (currentPlayer == 1 && redPlayerType == 1) {
        findBestMove(boardSize, currentPlayer);
        currentPlayer = 1 - currentPlayer;
        return;
    }
    else {
        return;
    }

}

void Game::findBestMove(int size, int currentPlayer)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Check each cell in the board
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            // Check horizontal patterns
            if (col + 2 < size) { // Ensure enough columns
                // Pattern SXS
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    gameBoard[row][col + 1] == EMPTY &&
                    (gameBoard[row][col + 2] == S_BLUE || gameBoard[row][col + 2] == S_RED)) {
                    gameBoard[row][col + 1] = getCurrentPlayerSymbol(currentPlayer, 1, 1);
                    UpdateScore(currentPlayer);
                    return;
                }
                // Pattern SOX
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    (gameBoard[row][col + 1] == O_BLUE || gameBoard[row][col + 1] == O_RED) &&
                    gameBoard[row][col + 2] == EMPTY) {
                    gameBoard[row][col + 2] = getCurrentPlayerSymbol(currentPlayer, 0, 0);
                    UpdateScore(currentPlayer);
                    return;
                }
                // Pattern XOS
                if (gameBoard[row][col] == EMPTY &&
                    (gameBoard[row][col + 1] == O_BLUE || gameBoard[row][col + 1] == O_RED) &&
                    (gameBoard[row][col + 2] == S_BLUE || gameBoard[row][col + 2] == S_RED)) {
                    gameBoard[row][col] = getCurrentPlayerSymbol(currentPlayer, 0, 0);
                    UpdateScore(currentPlayer);
                    return;
                }
            }

            // Check vertical patterns
            if (row + 2 < size) { // Ensure enough rows
                // Pattern SXS
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    gameBoard[row + 1][col] == EMPTY &&
                    (gameBoard[row + 2][col] == S_BLUE || gameBoard[row + 2][col] == S_RED)) {
                    gameBoard[row + 1][col] = getCurrentPlayerSymbol(currentPlayer, 1, 1);
                    UpdateScore(currentPlayer);
                    return;
                }
                // Pattern SOX
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    (gameBoard[row + 1][col] == O_BLUE || gameBoard[row + 1][col] == O_RED) &&
                    gameBoard[row + 2][col] == EMPTY) {
                    gameBoard[row + 2][col] = getCurrentPlayerSymbol(currentPlayer, 0, 0);
                    UpdateScore(currentPlayer);
                    return;
                }
                // Pattern XOS
                if (gameBoard[row][col] == EMPTY &&
                    (gameBoard[row + 1][col] == O_BLUE || gameBoard[row + 1][col] == O_RED) &&
                    (gameBoard[row + 2][col] == S_BLUE || gameBoard[row + 2][col] == S_RED)) {
                    gameBoard[row][col] = getCurrentPlayerSymbol(currentPlayer, 0, 0);
                    UpdateScore(currentPlayer);
                    return;
                }
            }

            // Check diagonal (top-left to bottom-right)
            if (row + 2 < size && col + 2 < size) {
                // Pattern SXS
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    gameBoard[row + 1][col + 1] == EMPTY &&
                    (gameBoard[row + 2][col + 2] == S_BLUE || gameBoard[row + 2][col + 2] == S_RED)) {
                    gameBoard[row + 1][col + 1] = getCurrentPlayerSymbol(currentPlayer, 1, 1);
                    UpdateScore(currentPlayer);
                    return;
                }
                // Pattern SOX
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    (gameBoard[row + 1][col + 1] == O_BLUE || gameBoard[row + 1][col + 1] == O_RED) &&
                    gameBoard[row + 2][col + 2] == EMPTY) {
                    gameBoard[row + 2][col + 2] = getCurrentPlayerSymbol(currentPlayer, 0, 0);
                    UpdateScore(currentPlayer);
                    return;
                }
                // Pattern XOS
                if (gameBoard[row][col] == EMPTY &&
                    (gameBoard[row + 1][col + 1] == O_BLUE || gameBoard[row + 1][col + 1] == O_RED) &&
                    (gameBoard[row + 2][col + 2] == S_BLUE || gameBoard[row + 2][col + 2] == S_RED)) {
                    gameBoard[row][col] = getCurrentPlayerSymbol(currentPlayer, 0, 0);
                    UpdateScore(currentPlayer);
                    return;
                }
            }

            // Check diagonal (bottom-left to top-right)
            if (row - 2 >= 0 && col + 2 < size) {
                // Pattern SXS
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    gameBoard[row - 1][col + 1] == EMPTY &&
                    (gameBoard[row - 2][col + 2] == S_BLUE || gameBoard[row - 2][col + 2] == S_RED)) {
                    gameBoard[row - 1][col + 1] = getCurrentPlayerSymbol(currentPlayer, 1, 1);
                    UpdateScore(currentPlayer);
                    return;
                }
                // Pattern SOX
                if ((gameBoard[row][col] == S_BLUE || gameBoard[row][col] == S_RED) &&
                    (gameBoard[row - 1][col + 1] == O_BLUE || gameBoard[row - 1][col + 1] == O_RED) &&
                    gameBoard[row - 2][col + 2] == EMPTY) {
                    gameBoard[row - 2][col + 2] = getCurrentPlayerSymbol(currentPlayer, 0, 0);
                    UpdateScore(currentPlayer);
                    return;
                }
                // Pattern XOS
                if (gameBoard[row][col] == EMPTY &&
                    (gameBoard[row - 1][col + 1] == O_BLUE || gameBoard[row - 1][col + 1] == O_RED) &&
                    (gameBoard[row - 2][col + 2] == S_BLUE || gameBoard[row - 2][col + 2] == S_RED)) {
                    gameBoard[row][col] = getCurrentPlayerSymbol(currentPlayer, 0, 0);
                    UpdateScore(currentPlayer);
                    return;
                }
            }
        }
    }

    // No patterns found - Place in random cell
    while (true) {
        int randRow = std::rand() % size;
        int randCol = std::rand() % size;
        if (gameBoard[randRow][randCol] == EMPTY) {
            int place = std::rand() % 2; // Randomly choose 0 (S) or 1 (O)
            gameBoard[randRow][randCol] = getCurrentPlayerSymbol(currentPlayer, place, place);
            UpdateScore(currentPlayer);
            return;
        }
    }
}

