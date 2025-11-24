#pragma once

#include "config.h"
#include "cell.h"
#include <vector>


class Solver {
protected:
    std::vector<Cell>& grid;
    Cell* endNode;
    Cell* current = nullptr;
    bool finished = false;

public:
    Solver(std::vector<Cell>& masterGrid, Cell* start, Cell* end) : grid(masterGrid), endNode(end) {}
    virtual ~Solver() = default;

    virtual bool step() = 0;

    Cell* getCurrent() {
        return current;
    }
};