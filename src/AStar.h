#pragma once

#include "config.h"
#include "solver.h"
#include "cell.h"

#include <vector>
#include <functional>
#include <queue>
#include <cmath>
#include <climits>

struct CellComparator {  //comparator for priority queue
    bool operator()(Cell* a, Cell* b) {
        if (a->fCost == b->fCost) {
            return a->hCost > b->hCost;
        }
        return a->fCost > b->fCost;
    }
};

using Node = std::pair<int, Cell*>;

class AStar : public Solver {
private:
    int weight = 4;

    //std::priority_queue<Cell*, std::vector<Cell*>, CellComparator> openSet; //cells we need to visit
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;
    std::vector<Cell*> closedSet; // cells we have already processed

    int manhattanDistance(Cell* cell, Cell* end) {
        return abs(cell->col - end->col) + abs(cell->row - end->row);
    }

    static int getIndex(int i, int j) {
        if (i < 0 || i > Config::COLUMNS - 1) return -1;
        if (j < 0 || j > Config::ROWS - 1) return -1;

        return j * Config::COLUMNS + i;
    }

public:
    AStar(std::vector<Cell>& mG, Cell *s, Cell *e) : Solver(mG, s, e) {
        for (auto& cell : grid) {
            cell.gCost = INT_MAX;
            cell.fCost = INT_MAX;
            cell.visited_bfs = false;
        }

        s->gCost = 0;
        s->hCost = manhattanDistance(s, e);
        s->fCost = s->gCost + s->hCost;

        //openSet.push(s);
        openSet.push({s->fCost, s});
    }

    bool step() override {
        if (openSet.empty()) {
            return true;
        }

        current = openSet.top().second;
        openSet.pop();

        if (current->visited_bfs == true) return false;

        current->visited_bfs = true; // use this for drawing || not for algorithm

        if (current == endNode) {
            return true;
        }

        closedSet.push_back(current);

        int tempG = current->gCost + 1;

        if (!current->walls[0]) {
            int index = getIndex(current->col, current->row - 1); // to use grid[index] for neightbor

            if (index != -1 && !grid[index].visited_bfs) {
                if (tempG < grid[index].gCost) {
                    grid[index].gCost = tempG;
                    grid[index].hCost = manhattanDistance(&grid[index], endNode);
                    grid[index].fCost = grid[index].gCost + grid[index].hCost * weight;
                    grid[index].parent = current;

                    //openSet.push(&grid[index]);
                    openSet.push({grid[index].fCost, &grid[index]});
                }
            }
        }
        if (!current->walls[1]) {
            int index = getIndex(current->col + 1, current->row);

            if (index != -1 && !grid[index].visited_bfs) {
                if (tempG < grid[index].gCost) {
                    grid[index].gCost = tempG;
                    grid[index].hCost = manhattanDistance(&grid[index], endNode);
                    grid[index].fCost = grid[index].gCost + grid[index].hCost * weight;
                    grid[index].parent = current;

                    //openSet.push(&grid[index]);
                    openSet.push({grid[index].fCost, &grid[index]});
                }
            }
        }
        if (!current->walls[2]) {
            int index = getIndex(current->col, current->row + 1);

            if (index != -1 && !grid[index].visited_bfs) {
                if (tempG < grid[index].gCost) {
                    grid[index].gCost = tempG;
                    grid[index].hCost = manhattanDistance(&grid[index], endNode);
                    grid[index].fCost = grid[index].gCost + grid[index].hCost * weight;
                    grid[index].parent = current;

                    //openSet.push(&grid[index]);
                    openSet.push({grid[index].fCost, &grid[index]});
                }
            }
        }
        if (!current->walls[3]) {
            int index = getIndex(current->col - 1, current->row);

            if (index != -1 && !grid[index].visited_bfs) {
                if (tempG < grid[index].gCost) {
                    grid[index].gCost = tempG;
                    grid[index].hCost = manhattanDistance(&grid[index], endNode);
                    grid[index].fCost = grid[index].gCost + grid[index].hCost * weight;
                    grid[index].parent = current;

                    //openSet.push(&grid[index]);
                    openSet.push({grid[index].fCost, &grid[index]});
                }
            }
        }

        return false;
    }
};