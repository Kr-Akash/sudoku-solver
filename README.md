# sudoku-solver
Sudoku 9x9 solver in C++ using bitmasks constraint tracking, pre recursion naked sinlge propagtion and MRV heuristic backtracking.
## Repository Structure
- `sudoku.cpp` -`SudokuSolver` class and `main()` reading a 9x9 grid from standard input.
- `input.txt` - Sample 9x9 Sudoku puzzle (`0` represents empty cells).
##
```bash
g++ sudoku.cpp -std=c++11
./a.out < input.txt
