#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "solver.hpp"
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <type_traits>

template <typename T>
void randomize(std::vector<T> &v, uint8_t level = 10){
	srand(time(0));
	for (int i = 0; i < level; ++i)
	{
		int pos1 = rand()%v.size();
		int pos2 = rand()%v.size();

		std::iter_swap(v.begin()+pos1, v.begin()+pos2);
	}
}

void fillRandomGrid(gridStore &g);

void generatePuzzle(gridStore &g, int emptyCells);

#endif