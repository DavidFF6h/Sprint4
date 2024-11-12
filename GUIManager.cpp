#include "GUIManager.h"
#include "ImGUI/imgui.h"

void GUIManager::renderGUI(int& currentPlayer, int& selected_radio1, int& selected_radio2, bool& newGameTriggered, int& dummyBoardSize, int& dummyGameMode, int gameMode, Game& game, int& bluePlayerType, int& redPlayerType) {
    ImGui::Begin("SOS Game Panel", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    ImGui::Text("Game Mode");
    ImGui::Spacing();
   
    ImGui::RadioButton("Simple", &dummyGameMode, 0);
    ImGui::RadioButton("General", &dummyGameMode, 1);

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
    ImGui::Text("Board Size");

    ImGui::Spacing();
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderInt("##BoardSize", &dummyBoardSize, 3, 9);

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    if (ImGui::Button("New Game", ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
        newGameTriggered = true;
        currentPlayer = 0; // Reset player to Blue
    }

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    if(!game.gameOver){
    ImGui::Text("Current Player: ");
    ImGui::Text(currentPlayer == 0 ? "Blue" : "Red");
    }
    else
    {
        switch (game.GetWinner())
        {
        case 0:
            ImGui::Text("Blue Wins!");
            break;
        case 1:
            ImGui::Text("Red Wins!");
            break;
        case 2:
            ImGui::Text("Draw!");
            break;
        default:
            ImGui::Text("Issue in determining winner");
            break;
        }
    }

    ImGui::Spacing();ImGui::Separator();ImGui::Spacing();

    ImGui::Text("Blue Player");
    if (gameMode == 1) {
        ImGui::SameLine(0, ImGui::GetContentRegionAvail().x / 4);
        ImGui::Text("Score: %d", game.GetBlueScore());
    }
    ImGui::Spacing();
    ImGui::RadioButton("S##Blue", &selected_radio1, 0);
    ImGui::SameLine(0, ImGui::GetContentRegionAvail().x / 2);
    ImGui::RadioButton("Human##Blue", &bluePlayerType, 0);
    ImGui::RadioButton("O##Blue", &selected_radio1, 1);
    ImGui::SameLine(0, ImGui::GetContentRegionAvail().x / 2);
    ImGui::RadioButton("CPU##Blue", &bluePlayerType, 1);

    ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

    ImGui::Text("Red Player");
    if (gameMode == 1) {
        ImGui::SameLine(0, (ImGui::GetContentRegionAvail().x / 4) + 5);
        ImGui::Text("Score: %d", game.GetRedScore());
    }
    ImGui::Spacing();
    ImGui::RadioButton("S##Red", &selected_radio2, 0);
    ImGui::SameLine(0, ImGui::GetContentRegionAvail().x / 2);
    ImGui::RadioButton("Human##Red", &redPlayerType, 0);
    ImGui::RadioButton("O##Red", &selected_radio2, 1);
    ImGui::SameLine(0, ImGui::GetContentRegionAvail().x / 2);
    ImGui::RadioButton("CPU##Red", &redPlayerType, 1);
    ImGui::End();
}

void GUIManager::renderGameBoard(Game& game, int& currentPlayer, int selected_radio1, int selected_radio2, int newBoardSize) {
    ImGui::Begin("Game Board", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

    float availableWidth = ImGui::GetContentRegionAvail().x;
    float availableHeight = ImGui::GetContentRegionAvail().y;
    float buttonSize = fmin((availableWidth / newBoardSize) - 10, (availableHeight / newBoardSize) - 10);
    float totalGridWidth = buttonSize * newBoardSize;
    float totalGridHeight = buttonSize * newBoardSize;
    float paddingX = (availableWidth - totalGridWidth) / 2.0f;
    float paddingY = (availableHeight - totalGridHeight) / 2.0f;

    ImGuiIO& io = ImGui::GetIO();
    ImGui::PushFont(io.Fonts->Fonts[1]);

    const auto& gameBoard = game.getGameBoard();
    for (int row = 0; row < newBoardSize; row++) {
        for (int col = 0; col < newBoardSize; col++) {
            const char* label = " ";
            bool colorPushed = false;

            if (gameBoard[row][col] == S_BLUE) {
                label = "S";
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                colorPushed = true;
            }
            else if (gameBoard[row][col] == O_BLUE) {
                label = "O";
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                colorPushed = true;
            }
            else if (gameBoard[row][col] == S_RED) {
                label = "S";
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                colorPushed = true;
            }
            else if (gameBoard[row][col] == O_RED) {
                label = "O";
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                colorPushed = true;
            }

            ImGui::PushID(row * newBoardSize + col);
            if (ImGui::Button(label, ImVec2(buttonSize, buttonSize))) {
                if (game.setCellState(row, col, currentPlayer, selected_radio1, selected_radio2)) 
                {
                    currentPlayer = 1 - currentPlayer;
                }
            }
            ImGui::PopID();

            if (colorPushed) {
                ImGui::PopStyleColor();
            }

            if (col < newBoardSize - 1) {
                ImGui::SameLine();
            }
        }
    }

    ImGui::PopFont();
    ImGui::End();
}
