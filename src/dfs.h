#pragma once

#include <vector>
#include <stack>
#include "config.h"
#include "solver.h"
#include "cell.h"

class DFS : public Solver {
private:
    std::stack<Cell*> stack;

    static int getIndex(int i, int j) {
        if (i < 0 || i > Config::COLUMNS - 1) return -1;
        if (j < 0 || j > Config::ROWS - 1) return -1;

        return j * Config::COLUMNS + i;
    }

public:
    DFS(std::vector<Cell>& mG, Cell *s, Cell *e) : Solver(mG, s, e) {
        stack.push(s);
    }

    bool step() override {
        while (!stack.empty()) {
            current = stack.top();

            if (!current->visited_bfs) {
                break;
            }

            stack.pop();
        }

        if (stack.empty()) {
            return true;
        }

        current->visited_bfs = true;

        if (current == endNode) {
            return true;
        }

        if (!current->walls[0]) {
            int index = getIndex(current->col, current->row - 1);
            if (index != -1 && !grid[index].visited_bfs) {
                Cell *neighbor = &grid[index];
                neighbor->parent = current;
                stack.push(neighbor);
            }
        }

        if (!current->walls[1]) {
            int index = getIndex(current->col + 1, current->row);
            if (index != -1 && !grid[index].visited_bfs) {
                Cell *neighbor = &grid[index];
                neighbor->parent = current;
                stack.push(neighbor);
            }
        }

        if (!current->walls[2]) {
            int index = getIndex(current->col, current->row + 1);
            if (index != -1 && !grid[index].visited_bfs) {
                Cell *neighbor = &grid[index];
                neighbor->parent = current;
                stack.push(neighbor);
            }
        }

        if (!current->walls[3]) {
            int index = getIndex(current->col - 1, current->row);
            if (index != -1 && !grid[index].visited_bfs) {
                Cell *neighbor = &grid[index];
                neighbor->parent = current;
                stack.push(neighbor);
            }
        }

        return false;
    }
};