#include "solver.hpp"
#include <wx/wx.h>

void printGrid(gridStore &g){

	wxPrintf("\n");
	for(int j = 0; j < 9; j++){
		for(int i = 0; i < 9; i++){
			wxPrintf("%d ", (int)g[j][i]);
			if((i+1)%3 == 0){
				wxPrintf(" ");
			}
		}
		wxPrintf("\n");
		if((j+1)%3 == 0){
			wxPrintf("\n");
		}
	}
}

bool possible(uint8_t n, uint8_t row, uint8_t col, const gridStore g){
	if(n > 9 || n < 1)
		return true;
	for(int c = 0; c < 9; c++){
		if(c != col && g[row][c] == n)
			return false;
	}
	for(int r = 0; r < 9; r++){
		if(r != row && g[r][col] == n)
			return false;
	}
	uint8_t scol = col/3;
	uint8_t srow = row/3;
	scol *= 3;
	srow *= 3;
	for(int r = 0; r < 3;r++){
		for(int c = 0; c < 3; c++){
			if(scol+c != col && srow+r != row && g[srow+r][scol+c] == n)
				return false;
		}
	}
	return true;
}

bool solve(uint8_t row, uint8_t col, gridStore g, std::vector<gridStore> &solutions, int maxSolutions){
	// std::cout<<"("<<(int)x<<","<<(int)y<<")\n";
	// printGrid(g);

	int c = col, r = row;
	for(; r < 9; r++){
		for(c = 0; c < 9; c++){
			if(g[r][c] == 0){
//				if(co < 10){
					// printGrid(g);
					// wxPrintf("solving... (%d, %d) = %d\n", (int)i, (int)j, (int)g[j][i]);
//					co += 1;
//				}
				for(int n = 1; n < 10; n++){
					if(possible(n, r, c, g)){
						g[r][c] = n;
						if(c == 8){
							if(!solve(r+1, 0, g, solutions, maxSolutions))
								return false;
						}else{
							if(!solve(r, c+1, g, solutions, maxSolutions))
								return false;
						}
					}
				}
				return true;
			}
		}
	}
	if(r > 8){
		solutions.push_back(g);
		if(solutions.size() >= maxSolutions)
			return false;
	}
	return true;
}