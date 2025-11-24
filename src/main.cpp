#include <raylib.h>
#include <iostream>
#include <string>

#include "imgui.h"
#include "rlImGui.h"

#include "config.h"
#include "maze.h"

int main() {

    InitWindow(Config::WIDTH, Config::HEIGHT, "Maze");
    SetTargetFPS(Config::FPS);
    ToggleFullscreen();

    Maze maze;

    rlImGuiSetup(true);

    //int generationSpeed = 0; // 0 - fast, 1 - slow
    //const char* generationSpeedOoptions[] = {"Slow", "Fast"};
    int currentAlgo = 0; //0 - bfs, 1 - dfs
    const char* algoOptions[] = {"BFS", "DFS", "A*"};

    int frameCounter = 0;
    int stepSpeedSlider = 1;
    int stepSpeed = 1;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
            CloseWindow();
            return 0;
        }

        frameCounter++;
        if (frameCounter >= stepSpeed) {
            maze.update();
            frameCounter = 0;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        maze.draw();

        rlImGuiBegin();
        ImGui::Begin("Controls");
        ImGui::Text("Settings");

        if (ImGui::Button("Instant Generation")) {
            maze.instant_generation();
        }

        ImGui::SliderInt("Speed", &stepSpeedSlider, 1, 10);
        stepSpeed = -stepSpeedSlider + 11;

        ImGui::Separator();

        ImGui::Text("Solver");

        ImGui::Combo("Algorithm", &currentAlgo, algoOptions, 3);

        if (ImGui::Button("Start")) {
            maze.startSolving(currentAlgo);
        }

        if (ImGui::Button("Instant Solution")) {
            maze.instant_solution();
        }

        std::string st = std::to_string(maze.getState());
        if (st == "0") ImGui::Text("Generating");
        else if (st == "1") ImGui::Text("IDLE");
        else if (st == "2") ImGui::Text("SOLVING");
        else if (st == "3") ImGui::Text("FINISHED");

        if (ImGui::Button("Reset")) {
            maze.reset();
        }

        ImGui::End();
        rlImGuiEnd();

        DrawFPS(10, 10);
        EndDrawing();

    }
    CloseWindow();

    return 0;
}
