#include <iostream>
#include <fstream>


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

int main(int argc, char* argv[]){
    if(!argument_validation(argc, argv)){
        return 0;
    }

    

}