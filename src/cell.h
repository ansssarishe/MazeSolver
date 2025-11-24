#pragma once
#include "config.h"
#include <raylib.h>

struct Cell {
    int col, row;
    bool visited = false;
    bool visited_bfs = false;

    int gCost = INT_MAX;
    int hCost = 0;
    int fCost = 0;

    Cell* parent = nullptr;

    bool walls[4] = {true, true, true, true};

    void draw() const {
        int x = col * Config::CELL_SIZE;
        int y = row * Config::CELL_SIZE;
        int s = Config::CELL_SIZE;

        if (visited) {
            DrawRectangle(x, y, s, s, Config::VISITED_COLOR);
        }

        if (walls[0]) DrawLine(x, y, x + s, y, Config::WALL_COLOR);
        if (walls[1]) DrawLine(x + s, y, x + s, y + s, Config::WALL_COLOR);
        if (walls[2]) DrawLine(x, y + s, x + s, y + s, Config::WALL_COLOR);
        if (walls[3]) DrawLine(x, y, x, y + s, Config::WALL_COLOR);
    }

    void drawHighlight() const {
        int x = col * Config::CELL_SIZE;
        int y = row * Config::CELL_SIZE;
        int s = Config::CELL_SIZE;

        DrawRectangle(x, y, s, s, Config::HEAD_COLOR);
    }
};