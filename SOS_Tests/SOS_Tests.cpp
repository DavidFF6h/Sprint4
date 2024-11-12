#include "pch.h"
#include "CppUnitTest.h"
#include "../GeneralGame.h"
#include "../GeneralGame.cpp"
#include "../Game.h"
#include "../Game.cpp"
#include "../CellState.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {
    namespace VisualStudio {
        namespace CppUnitTestFramework {

            template<> inline std::wstring ToString<CellState>(const CellState& cs) {
                switch (cs) {
                case EMPTY: return L"EMPTY";
                case S_BLUE: return L"S_BLUE";
                case S_RED: return L"S_RED";
                case O_BLUE: return L"O_BLUE";
                case O_RED: return L"O_RED";
                default: return L"Unknown";
                }
            }

        }
    }
}

namespace SOSTests
{
    TEST_CLASS(GameTests)
    {
    public:

        GeneralGame* game;
        int currentPlayer;
        int bluePlayerType;
        int redPlayerType;

        TEST_METHOD_INITIALIZE(Setup)
        {
            game = new GeneralGame(3);
            currentPlayer = 0;
            bluePlayerType = 1;
            redPlayerType = 1;
            game->gameOver = false;
        }

        TEST_METHOD_CLEANUP(Cleanup)
        {
            delete game;
            game = nullptr;
        }

        TEST_METHOD(FindBestMove_SXS_Pattern)
        {
            game->gameBoard[0][0] = S_BLUE;
            game->gameBoard[0][1] = EMPTY;
            game->gameBoard[0][2] = S_BLUE;

            game->findBestMove(3, currentPlayer);
            Assert::AreEqual(game->getCurrentPlayerSymbol(currentPlayer, 1, 1), game->gameBoard[0][1]);
        }

        TEST_METHOD(FindBestMove_SOX_Pattern)
        {
            game->gameBoard[0][0] = S_BLUE;
            game->gameBoard[0][1] = O_BLUE;
            game->gameBoard[0][2] = EMPTY;

            game->findBestMove(3, currentPlayer);
            Assert::AreEqual(game->getCurrentPlayerSymbol(currentPlayer, 0, 0), game->gameBoard[0][2]);
        }

        TEST_METHOD(FindBestMove_XOS_Pattern)
        {
            game->gameBoard[0][0] = EMPTY;
            game->gameBoard[0][1] = O_BLUE;
            game->gameBoard[0][2] = S_BLUE;

            game->findBestMove(3, currentPlayer);
            Assert::AreEqual(game->getCurrentPlayerSymbol(currentPlayer, 0, 0), game->gameBoard[0][0]);
        }

        TEST_METHOD(FindBestMove_NoPattern_RandomPlacement)
        {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    game->gameBoard[i][j] = S_BLUE;
                }
            }
            game->gameBoard[1][1] = EMPTY;

            game->findBestMove(3, currentPlayer);
            Assert::IsTrue(game->gameBoard[1][1] != EMPTY);
        }
    };
}


