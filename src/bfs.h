#pragma once

#include <vector>
#include <queue>
#include "config.h"
#include "solver.h"
#include "cell.h"

class BFS : public Solver {
private:
    std::queue<Cell *> queue;

    static int getIndex(int i, int j) {
        if (i < 0 || i > Config::COLUMNS - 1) return -1;
        if (j < 0 || j > Config::ROWS - 1) return -1;

        return j * Config::COLUMNS + i;
    }

public:
    BFS(std::vector<Cell>& mG, Cell *s, Cell *e) : Solver(mG, s, e) {
        queue.push(s);
        s->visited_bfs = true;
    }

    bool step() override {
        if (queue.empty()) {
            return true;
        }

        current = queue.front();
        queue.pop();

        if (current == endNode) {
            return true;
        }

        //current->visited_bfs = true;

        if (!current->walls[0]) {
            int index = getIndex(current->col, current->row - 1);

            if (index != -1 && !grid[index].visited_bfs) {
                Cell *neighbor = &grid[index];

                neighbor->visited_bfs = true;
                neighbor->parent = current;

                queue.push(neighbor);
            }
        }
        if (!current->walls[1]) {
            int index = getIndex(current->col + 1, current->row);

            if (index != -1 && !grid[index].visited_bfs) {
                Cell *neighbor = &grid[index];

                neighbor->visited_bfs = true;
                neighbor->parent = current;

                queue.push(neighbor);
            }
        }
        if (!current->walls[2]) {
            int index = getIndex(current->col, current->row + 1);

            if (index != -1 && !grid[index].visited_bfs) {
                Cell *neighbor = &grid[index];

                neighbor->visited_bfs = true;
                neighbor->parent = current;

                queue.push(neighbor);
            }
        }
        if (!current->walls[3]) {
            int index = getIndex(current->col - 1, current->row);

            if (index != -1 && !grid[index].visited_bfs) {
                Cell *neighbor = &grid[index];

                neighbor->visited_bfs = true;
                neighbor->parent = current;

                queue.push(neighbor);
            }
        }

        return false;
    }
};
