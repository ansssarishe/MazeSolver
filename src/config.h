#pragma once
#include <raylib.h>

namespace Config {
    // -----Config-----
    constexpr int WIDTH = 1440;
    constexpr int HEIGHT = 900;
    constexpr int FPS = 60;

    constexpr int CELL_SIZE = 20;
    constexpr int COLUMNS = WIDTH / CELL_SIZE;
    constexpr int ROWS = HEIGHT / CELL_SIZE;

    const Color WALL_COLOR = WHITE;
    const Color VISITED_COLOR = {255, 100, 167, 255};
    const Color HEAD_COLOR = GREEN;
};