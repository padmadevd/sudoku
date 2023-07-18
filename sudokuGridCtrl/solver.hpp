#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <iostream>
#include <sys/types.h>
#include <array>
#include <vector>

typedef std::array<std::array<uint8_t, 9>, 9> gridStore;

void printGrid(gridStore &g);

bool possible(uint8_t n, uint8_t row, uint8_t col, const gridStore g);

bool solve(uint8_t row, uint8_t col, gridStore g, std::vector<gridStore> &solutions, int maxSolutions);

#endif