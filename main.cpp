#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include "Game.h"
#include "SimpleGame.h"
#include "GeneralGame.h"
#include "GUIManager.h"

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    if (!glfwInit()) {
        printf("GLFW initialization failed.\n");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "SOS Game", NULL, NULL);
    if (!mainWindow) {
        printf("GLFW window creation failed.\n");
        glfwTerminate();
        return 1;
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed.\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, bufferWidth, bufferHeight);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(mainWindow, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("Fonts/PixelifySans-Medium.ttf", 64.0f);

    Game* game = nullptr;
    game = new SimpleGame(3);


    GUIManager guiManager;

    int currentPlayer = 0;
    int selected_radio1 = 0;
    int selected_radio2 = 0;
    bool newGameTriggered = false;
    int dummyBoardSize = 3;
    int newBoardSize = 3;
    int dummyGameMode = 0;
    int newGameMode = 0;
    int dummyBluePlayerType = 0;
    int bluePlayerType = 0;
    int dummyRedPlayerType = 0;
    int redPlayerType = 0;

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int window_width, window_height;
        glfwGetWindowSize(mainWindow, &window_width, &window_height);

        float left_fourth_width = window_width / 4.0f;
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(left_fourth_width, window_height));
        guiManager.renderGUI(currentPlayer, selected_radio1, selected_radio2, newGameTriggered, dummyBoardSize, dummyGameMode, newGameMode, *game, dummyBluePlayerType, dummyRedPlayerType);

        game->TryCPUMove(currentPlayer, bluePlayerType, redPlayerType);

        if (newGameTriggered) {
            newBoardSize = dummyBoardSize;
            newGameMode = dummyGameMode;
            bluePlayerType = dummyBluePlayerType;
            redPlayerType = dummyRedPlayerType;
            switch (newGameMode)
            {
            case 0:
                game = new SimpleGame(newBoardSize);
                break;
            case 1:
                game = new GeneralGame(newBoardSize);
                break;
            default:
                printf("Error in game mode selection");
                return -1;
            }
            printf("Game Mode: %d\n", newGameMode);
            printf("Blue Player Type: %d\n", bluePlayerType);
            printf("Red Player Type: %d\n", redPlayerType);
            game->resetGame(newBoardSize);
            newGameTriggered = false;
        }

        ImGui::SetNextWindowPos(ImVec2(window_width / 4, 0));
        ImGui::SetNextWindowSize(ImVec2(window_width * 3 / 4, window_height));
        guiManager.renderGameBoard(*game, currentPlayer, selected_radio1, selected_radio2, newBoardSize);

        glClearColor(0.13f, 0.17f, 0.21f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(mainWindow);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(mainWindow);
    glfwTerminate();

    return 0;
}