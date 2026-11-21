#include<vector>
#include<iostream>

using namespace std;

class SudokuSolver{
	private:
	static constexpr int GRID_SIZE = 9; //9x9
	static constexpr int SUBGRID_SIZE = 3; //3x3
	static constexpr int ALL_MASK = 0x1FF; //first 9bits are set 1 for each [9 8 7...3 2 1]
	
	struct Cell{
		int row, col;
		Cell(int r, int c): row(r), col(c){}
	};
	
	vector<vector<int>> &grid;
	vector<int> row_mask = vector<int>(GRID_SIZE, 0);
	vector<int> col_mask = vector<int>(GRID_SIZE, 0);
	vector<int> subgrid_mask = vector<int>(GRID_SIZE, 0);
	vector<Cell> empty;
	
	
	int subgrid(int r, int c) const{
		return (r/SUBGRID_SIZE)*SUBGRID_SIZE + (c/SUBGRID_SIZE);
	}
	
	void toggle(int r, int c, int bit, int val){
		row_mask[r] ^= bit;
		col_mask[c] ^= bit;
		subgrid_mask[subgrid(r,c)] ^= bit;
		grid[r][c] = val;
	}
	
	int candidates(int r, int c) const{
		return ~(row_mask[r] | col_mask[c] | subgrid_mask[subgrid(r,c)]) & ALL_MASK;
	}
	
	bool verify(){ //Assumed the values are always between 0-9 in each cell
		for(int r = 0; r<GRID_SIZE; ++r){
			for(int c = 0; c<GRID_SIZE; ++c){
				if(grid[r][c] == 0)
					empty.push_back({r,c});
				else{
					int bit = 1 << (grid[r][c]-1);
					if((row_mask[r] | col_mask[c] | subgrid_mask[subgrid(r,c)]) & bit)
						return false; //Duplicate value already present in some row/col/subgrid
					toggle(r, c, bit, grid[r][c]); //set the bit 
				}
			}
		}
		return true;
	}
	
	bool fillKnown(){ //if only one value is allowed in empty cell, then fill it and check again for changes
		bool flag = true;
		while(flag){
			flag = false;
			for(int i = 0; i<empty.size();){
				int mask = candidates(empty[i].row, empty[i].col);
				if(!mask)
					return false; //empty cell (0), has no allowed digit to be placed
				if(__builtin_popcount(mask) == 1){
					toggle(empty[i].row, empty[i].col, mask, __builtin_ctz(mask) +1); //set the only digit
					empty[i] = empty[empty.size()-1];
					empty.pop_back();
					flag = true;
				}
				else
					++i;
			}
		}
		return true;
	}
	
	bool fillUnknown(int index){ //each empty cell has now more than 1 value possible
		if(index == empty.size())
			return true; //all cells are filled 
		/* Minimum Remaing Values heuristic */
		int best = index, minimum = 10;
		for(int i = index; i<empty.size(); ++i){
			int allowed_mask = candidates(empty[i].row, empty[i].col);
			int allowed_digits = __builtin_popcount(allowed_mask);
			if(allowed_digits == 0)
				return false;
			if(minimum > allowed_digits){
				minimum = allowed_digits;
				best = i;
			}
			if(minimum == 1)
				break;
		}
		swap(empty[index], empty[best]);
		/*---------------------------------*/
		int allowed_mask = candidates(empty[index].row, empty[index].col);
		while(allowed_mask){
			int bit = allowed_mask & (-allowed_mask);
			toggle(empty[index].row, empty[index].col, bit, __builtin_ctz(bit)+1);
			if(fillUnknown(index+1))
				return true;
			toggle(empty[index].row, empty[index].col, bit, 0);
			allowed_mask ^= bit;
		}
		return false;
	}
	
	public:
	explicit SudokuSolver(vector<vector<int>> &board) : grid(board){}
	
	bool solve(){
		return verify() && fillKnown() && fillUnknown(0);
	}
	
};

void displayBoard(const vector<vector<int>> &board){
	for(int i = 0; i<board.size(); ++i){
		for(int j = 0; j<board[i].size(); ++j){
			cout << board[i][j] << " ";
		}
		cout << "\n";
	}
}

void inputBoard(vector<vector<int>> &board){
	for(int i = 0; i<board.size(); ++i){
		for(int j = 0; j<board[i].size(); ++j){
			cin >> board[i][j];
		}
	}
}

int main(){
	vector<vector<int>> board(9, vector<int>(9,0));
	inputBoard(board);
	SudokuSolver solver(board);
	if(solver.solve()){
		cout << "Solved board:\n";
		displayBoard(board);
	}
	else
		cout << "NO SOLUTION EXISTS!!!";
	return 0;
}