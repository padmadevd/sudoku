#include "generator.hpp"
#include "solver.hpp"
#include <wx/wx.h>
#include <cstdint>
#include <list>

void fillRandomGrid(gridStore &g){
	// wxPrintf("filling grid...\n");

	for (int r = 0; r < 9; ++r)
	{
		for (int c = 0; c < 9; ++c)
		{
			g[r][c] = 0;
		}
	}

	std::vector<uint8_t> nums;
	for (int i = 1; i < 10; ++i)
		nums.push_back(i);

	for (int sg = 0; sg < 3; ++sg)
	{
		randomize(nums);
		for (int r = 0; r < 3; ++r)
		{
			for (int c = 0; c < 3; ++c)
			{
				g[sg*3+r][sg*3+c] = nums[8-(r*3+c)];
			}
		}
	}
	// printGrid(g);
	std::vector<gridStore> solution;
	solve(0, 0, g, solution, 1);
	if(solution.empty())
		fillRandomGrid(g);
	else
		g = solution[0];
}

void generatePuzzle(gridStore &g, int emptyCells){
	fillRandomGrid(g);

	std::vector<int> positions;
	int pi = 0;
	for (int i = 0; i < 81; ++i)
	{
		positions.push_back(i);
	}
	randomize(positions, 100);
	bool found = false;
	while(emptyCells != 0){
		// wxPrintf("generating....\n");
		if(positions[pi] != -1){
			gridStore temp = g;
			int r = positions[pi]/9;
			int c = positions[pi]%9;
			temp[r][c] = 0;
			std::vector<gridStore> solutions;
			solve(0, 0, temp, solutions, 2);
			if(solutions.size() == 1){
				g[r][c] = 0;
				emptyCells -= 1;
				positions[pi] = -1;
				found = true;
			}
		}
		pi += 1;
		if(pi > 80){
			pi = 0;
			if (!found)
			{
				// wxPrintf("cant generate %d\n", emptyCells);
				return;
			}
			found = false;
		}
	}
}