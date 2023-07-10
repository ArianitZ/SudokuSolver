#include <iostream>
#include <fstream>
#include <array>

bool argument_validation(int argc, char* argv[]){
    if(argc < 2){
        std::cout   << "ERROR:\t A file containing a Sudoku puzzle must be provided." << "\n"
                    << "\t Example: ./bin/sudoku_solver assets/puzzle/example.txt" << "\n";
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

// Ensure grid is valid
bool check_grid_validity(std::array<std::array<int, 9>, 9>& grid){
    // Check all 3 by 3 grids
    int block_size{3};
    for(int i{0}; i < block_size; i++){
        for(int j{0}; j < block_size; j++){
            std::array<int, 10> cell_values{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
            for(int k{0}; k < 9; k++){
                int cell_value{ grid[i*block_size+k%block_size][j*block_size+k/block_size] };
                cell_values[cell_value] += 1;
                if(cell_value > 0 && cell_values[cell_value] > 1){
                    std::cout << "Grid not valid, block_size" << "\n";
                    return false;
                }
            }
        }
    }

    // Check all rows
    for(auto& row: grid){
        std::array<int, 10> cell_values{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
        for(auto & cell_value: row){
            cell_values[cell_value] += 1;
            if(cell_value > 0 && cell_values[cell_value] > 1){
                std::cout << "Grid not valid" << "\n";
                return false;
            }
        }
    }

    // Check all columns
    for(int j{0}; j < 9; j++){
        std::array<int, 10> cell_values{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        for(int i{0}; i < 9; i++){
            int cell_value = grid[i][j];
            cell_values[cell_value] += 1;
            if(cell_value > 0 && cell_values[cell_value] > 1){
                    std::cout << "Grid not valid" << "\n";
                    return false;
            }
        }
    }

    return true;
}

// Perform DFS

int main(int argc, char* argv[]){
    if(!argument_validation(argc, argv)){
        return 0;
    }
    std::ifstream puzzle(argv[1]);
    std::array<std::array<int, 9>, 9> grid;
    for(int i{0}; i < 9; i++){
        for(int j{0}; j < 9; j++){
            puzzle >> grid[i][j];
        }
    }
    check_grid_validity(grid);
    grid[0][0] = 8;
    grid[8][8] = 1;
    check_grid_validity(grid);
    for(int i{0}; i < 9; i++){
        for(int j{0}; j < 9; j++){
            std::cout << grid[i][j] << " ";
        }
        std::cout << "\n";
    }
    


}