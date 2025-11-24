#pragma once

#include <raylib.h>
#include <vector>
#include <stack>
#include <iostream>
#include <chrono>

#include "config.h"
#include "cell.h"
#include "bfs.h"
#include "dfs.h"
#include "AStar.h"
#include "solver.h"

enum MazeState {
    GENERATING,
    IDLE,
    SOLVING,
    FINISHED
};

std::chrono::time_point<std::chrono::steady_clock> time_start, time_end;

class Maze {
private:
    std::vector<Cell> grid;
    std::stack<Cell*> stack;
    Cell* current;

    MazeState state;

    Solver* solver = nullptr;

    Cell* start{};
    Cell* end{};

    static int getIndex(int i, int j) {
        if (i < 0 || i > Config::COLUMNS - 1) return -1;
        if (j < 0 || j > Config::ROWS - 1) return -1;

        return j * Config::COLUMNS + i;
    }

public:
    Maze() {
        grid.reserve(Config::COLUMNS * Config::ROWS);
        for (int j = 0; j < Config::ROWS; j++) {
            for (int i = 0; i < Config::COLUMNS; i++) {
                grid.push_back({i, j});
            }
        }

        state = GENERATING;

        current = &grid[0]; //starting in a top-left corner
        current->visited = true;
        stack.push(current);
    }

    ~Maze() {
        if (solver != nullptr) {
            delete solver;
            solver = nullptr;
        }
    }

    void instant_generation() {
        while (state == GENERATING) {
            update();
        }
    }

    void instant_solution() {
        while (state == SOLVING) {
            update();
        }
    }

    void startGenerating(int speed) {
        if (speed == 1) {
            while (state == GENERATING) {
                update();
            }
        }
        if (speed == 0) {
            state = GENERATING;
        }
    }

    void startSolving(int algoIndex) {
        if (state != IDLE) return;

        for (auto& cell : grid) {
            cell.visited_bfs = false; // Reset flag
            cell.parent = nullptr;    // Reset path
        }

        state = SOLVING;

        time_start = std::chrono::steady_clock::now();

        // Clean up old solver
        if (solver != nullptr) delete solver;

        start = &grid[0];
        end = &grid[grid.size() - 1];

        if (algoIndex == 0) {
            solver = new BFS(grid, start, end);
        }
        else if (algoIndex == 1) {
            solver = new DFS(grid, start, end);
        }
        else if (algoIndex == 2) {
            solver = new AStar(grid, start, end);
        }
    }

    void update() {
        switch (state) {

            case GENERATING: {
                if (stack.empty()) {
                    state = IDLE;
                    current = nullptr;

                    createLoops();
                    return;
                }

                Cell* next = checkNeighbors(current);
                if (next != nullptr) {
                    next->visited = true;
                    stack.push(current);
                    removeWalls(current, next);
                    current = next;
                }
                else {
                    if (!stack.empty()) {
                        current = stack.top();
                        stack.pop();
                    }
                }
                break;
            }
            case SOLVING: {

                if (solver != nullptr) {
                    bool finished = solver->step();

                    if (finished) {
                        state = FINISHED;
                        time_end = std::chrono::steady_clock::now();
                        std::chrono::duration<double> elapsed = time_end - time_start;
                        std::cout << "Time taken: " << elapsed.count() << std::endl;
                    }
                }

                break;
            }
        }
    }

    void draw() {
        for (const auto& cell : grid) {
            int x = cell.col * Config::CELL_SIZE;
            int y = cell.row * Config::CELL_SIZE;

            if (state == SOLVING && solver != nullptr && solver->getCurrent() == &cell) {
                DrawRectangle(x, y, Config::CELL_SIZE, Config::CELL_SIZE, RED);
            }
            else if (state == GENERATING && &cell == current) {
                DrawRectangle(x, y, Config::CELL_SIZE, Config::CELL_SIZE, Config::HEAD_COLOR);
            }
            else if (&cell == start) {
                DrawRectangle(x, y, Config::CELL_SIZE, Config::CELL_SIZE, GREEN);
            }
            else if (&cell == end) {
                DrawRectangle(x, y, Config::CELL_SIZE, Config::CELL_SIZE, RED);
            }
            else if (cell.visited_bfs) {
                DrawRectangle(x, y, Config::CELL_SIZE, Config::CELL_SIZE, BLUE);
            }
            else if (cell.visited) {
                DrawRectangle(x, y, Config::CELL_SIZE, Config::CELL_SIZE, BLACK);
            }
            else {
                DrawRectangle(x, y, Config::CELL_SIZE, Config::CELL_SIZE, BLACK);
            }

            // Walls
            Color wallColor = WHITE;
            if (cell.walls[0]) DrawLine(x, y, x + Config::CELL_SIZE, y, wallColor);
            if (cell.walls[1]) DrawLine(x + Config::CELL_SIZE, y, x + Config::CELL_SIZE, y + Config::CELL_SIZE, wallColor);
            if (cell.walls[2]) DrawLine(x, y + Config::CELL_SIZE, x + Config::CELL_SIZE, y + Config::CELL_SIZE, wallColor);
            if (cell.walls[3]) DrawLine(x, y, x, y + Config::CELL_SIZE, wallColor);
        }

        if (state == FINISHED && end != nullptr) {
            Cell* pathNode = end;

            int safetyCount = 0;
            int maxSteps = Config::COLUMNS * Config::ROWS;

            while (pathNode->parent != nullptr && safetyCount < maxSteps) {
                int x = pathNode->col * Config::CELL_SIZE;
                int y = pathNode->row * Config::CELL_SIZE;

                int x_p = pathNode->parent->col * Config::CELL_SIZE;
                int y_p = pathNode->parent->row * Config::CELL_SIZE;

                float new_x = static_cast<float>(x + Config::CELL_SIZE / 2);
                float new_y = static_cast<float>(y + Config::CELL_SIZE / 2);

                float new_x_p = static_cast<float>(x_p + Config::CELL_SIZE / 2);
                float new_y_p = static_cast<float>(y_p + Config::CELL_SIZE / 2);

                //DrawLine(x + Config::CELL_SIZE / 2, y + Config::CELL_SIZE / 2, x_p + Config::CELL_SIZE / 2, y_p + Config::CELL_SIZE / 2, RED);
                //DrawRectangle(x, y, Config::CELL_SIZE, Config::CELL_SIZE, GREEN);
                DrawLineEx({new_x, new_y}, {new_x_p, new_y_p}, 5, GREEN);

                pathNode = pathNode->parent; // Move backwards
                safetyCount++;
            }
        }
    }

    Cell* checkNeighbors(Cell* cell) {
        std::vector<Cell*> neighbors;
        int top = getIndex(cell->col, cell->row - 1);
        int right = getIndex(cell->col + 1, cell->row);
        int bottom = getIndex(cell->col, cell->row + 1);
        int left = getIndex(cell->col - 1, cell->row);

        if (top != -1 && !grid[top].visited) neighbors.push_back(&grid[top]);
        if (right != -1 && !grid[right].visited) neighbors.push_back(&grid[right]);
        if (bottom != -1 && !grid[bottom].visited) neighbors.push_back(&grid[bottom]);
        if (left != -1 && !grid[left].visited) neighbors.push_back(&grid[left]);

        if (neighbors.empty()) return nullptr;

        int r = GetRandomValue(0, neighbors.size() - 1);
        return neighbors[r];
    }

    static void removeWalls(Cell* current, Cell* next) {
        int dx = current->col - next->col;
        int dy = current->row - next->row;

        if (dx == 1) {
            current->walls[3] = false;
            next->walls[1] = false;
        }
        if (dx == -1) {
            current->walls[1] = false;
            next->walls[3] = false;
        }
        if (dy == 1) {
            current->walls[0] = false;
            next->walls[2] = false;
        }
        if (dy == -1) {
            current->walls[2] = false;
            next->walls[0] = false;
        }
    }

    void reset() {
        for (auto& cell : grid) {
            cell.visited = false;
            cell.visited_bfs = false; // Reset flag
            cell.parent = nullptr;    // Reset path

            state = IDLE;
        }
    }

    bool isFinished() {
        if (stack.empty()) return true;
        return false;
    }

    MazeState getState() {
        return state;
    }

    void createLoops() {
        for (int i = 0; i < grid.size(); i++) {
            Cell* cell = &grid[i];

            // 15% chance to remove a wall
            if (GetRandomValue(0, 100) < 15 ) {

                int wallIdx = GetRandomValue(0, 3);

                if (wallIdx == 0 && cell->row > 0) {
                    cell->walls[0] = false;
                    int idx = getIndex(cell->col, cell->row - 1);
                    grid[idx].walls[2] = false;
                }
                else if (wallIdx == 1 && cell->col < Config::COLUMNS - 1) {
                    cell->walls[1] = false;
                    int idx = getIndex(cell->col + 1, cell->row);
                    grid[idx].walls[3] = false;
                }
                else if (wallIdx == 2 && cell->row < Config::ROWS - 1) {
                    cell->walls[2] = false;
                    int idx = getIndex(cell->col, cell->row + 1);
                    grid[idx].walls[0] = false;
                }
                else if (wallIdx == 3 && cell->col > 0) {
                    cell->walls[3] = false;
                    int idx = getIndex(cell->col - 1, cell->row);
                    grid[idx].walls[1] = false;
                }
            }
        }
    }

};