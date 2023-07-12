#include <iostream>
#include <fstream>
#include <array>
#include <stack>

const int GRID_SIZE{9};

bool argument_validation(int argc, char* argv[]){
    if(argc < 2){
        std::cout   << "ERROR:\t A file containing a Sudoku puzzle must be provided." << "\n"
                    << "\t Example: ./bin/sudoku_sudoku_grid assets/puzzle/example.txt" << "\n";
        return false;
    }

    std::string filename{argv[1]};
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cout << "ERROR:\t Cannot open file: " << filename << "\n";
        return false;
    }
    file.close();

    return true;
}

std::array<std::array<int, GRID_SIZE>, GRID_SIZE> create_grid(const std::string& filepath){
    std::ifstream puzzle(filepath);

    std::array<std::array<int, GRID_SIZE>, GRID_SIZE> grid;
    for(int i{0}; i < GRID_SIZE; i++){
        for(int j{0}; j < GRID_SIZE; j++){
            puzzle >> grid[i][j];
        }
    }

    return grid;
}


bool check_row_validity(const std::array<std::array<int, GRID_SIZE>, GRID_SIZE>& grid){
    for(auto& row: grid){
        std::array<int, GRID_SIZE+1> cell_values{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
        for(auto& cell_value: row){
            cell_values[cell_value] += 1;
            if(cell_value > 0 && cell_values[cell_value] > 1){
                return false;
            }
        }
    }
    return true;
}

bool check_column_validity(const std::array<std::array<int, GRID_SIZE>, GRID_SIZE>& grid){
    for(int j{0}; j < GRID_SIZE; j++){
        std::array<int, GRID_SIZE+1> cell_values{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        for(int i{0}; i < GRID_SIZE; i++){
            int cell_value = grid[i][j];
            cell_values[cell_value] += 1;
            if(cell_value > 0 && cell_values[cell_value] > 1){
                    return false;
            }
        }
    }

    return true;
}

bool check_block_validity(const std::array<std::array<int, GRID_SIZE>, GRID_SIZE>& grid){
    int block_size{3};
    for(int i{0}; i < block_size; i++){
        for(int j{0}; j < block_size; j++){
            std::array<int, GRID_SIZE+1> cell_values{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
            for(int k{0}; k < GRID_SIZE; k++){
                int cell_value{ grid[i*block_size+k%block_size][j*block_size+k/block_size] };
                cell_values[cell_value] += 1;
                if(cell_value > 0 && cell_values[cell_value] > 1){
                    return false;
                }
            }
        }
    }

    return true;
}

bool check_grid_validity(const std::array<std::array<int, GRID_SIZE>, GRID_SIZE>& grid){
    return check_row_validity(grid) && check_column_validity(grid) && check_block_validity(grid);
}

struct SudokuGrid{
    std::array<std::array<int, GRID_SIZE>, GRID_SIZE> grid;
    int i, j; 
};

int get_new_number(const std::array<int, GRID_SIZE>& number_pool){
    for(int i{0}; i<GRID_SIZE; i++){
        if(number_pool[i] == 0){
            return i+1;
        }
    }
    return -1;
}

int check_win_condition(const std::array<std::array<int, GRID_SIZE>, GRID_SIZE>& grid){
    for(auto & row: grid){
        int sum{0};
        for(auto & value: row){
            sum += value;
        }
        if(sum != 45){
            return false;
        }
    }

    for(int j{0}; j < GRID_SIZE; j++){
        int sum{0};
        for(int i{0}; i < GRID_SIZE; i++){
            sum += grid[i][j];
        }
        if(sum != 45){
            return false;
        }
    }

    return true;
}

std::pair<int, int> find_zero_value(const std::array<std::array<int, GRID_SIZE>, GRID_SIZE>& grid){
    for(int i{0}; i<GRID_SIZE; i++){
        for(int j{0}; j<GRID_SIZE; j++){
            if(grid[j][i] == 0){
                return std::pair<int,int>{i,j};
            }
        }
    }
    return std::pair<int,int>{-1,-1};
}

void print_solution(const std::array<std::array<int, GRID_SIZE>, GRID_SIZE>& grid){
    std::cout << "Solution:" << "\n";
    for(int i{0}; i < GRID_SIZE; i++){
        if(i%3 == 0 && i>0){
            std::cout << "\t---------------------" << "\n";
        }
        std::cout << "\t";
        for(int j{0}; j < GRID_SIZE; j++){
            if(j%3 == 0 && j>0){
                std::cout << "| ";
            }
            std::cout << grid[i][j] << " ";
        }
        std::cout << "\n";
    }
}

int main(int argc, char* argv[]){
    if(!argument_validation(argc, argv)){
        return 0;
    }
    std::array<std::array<int, GRID_SIZE>, GRID_SIZE> grid = create_grid(argv[1]);

    SudokuGrid sudoku_grid{grid, 0, 0};
    std::stack<SudokuGrid> stack;
    stack.emplace(sudoku_grid);

    while(!stack.empty()){
        sudoku_grid = stack.top();
        stack.pop();
        
        if(!check_grid_validity(sudoku_grid.grid)){
            continue;
        }

        if(check_win_condition(sudoku_grid.grid)){
            break;
        }

        if(sudoku_grid.grid[sudoku_grid.j][sudoku_grid.i] == 0){
            for(int num{1}; num <= GRID_SIZE; num++){
                SudokuGrid new_sudoku_grid{sudoku_grid.grid, sudoku_grid.i, sudoku_grid.j};
                new_sudoku_grid.grid[new_sudoku_grid.j][new_sudoku_grid.i] = num;
                if(check_grid_validity(new_sudoku_grid.grid)){
                    stack.emplace(new_sudoku_grid);
                }
            }
        }
        else{
            std::pair<int,int> new_position{ find_zero_value(sudoku_grid.grid) };
            if(new_position.first > -1 && new_position.second > -1){
                SudokuGrid new_sudoku_grid{sudoku_grid.grid, new_position.first, new_position.second};
                stack.emplace(new_sudoku_grid);
            }
        }
    }

    print_solution(sudoku_grid.grid);
}