/*==============================================================================
# Title           : main.cpp of Task1App
# Description     : This is a c++ app that do the following:
#                       * Reads a CSV file from a chosen path.
#                       * Filters the 0 values in the data.
#                       * Writes the filtered values in another csv file
#                         in a chosen path.
# Author          : Salah Eddine Ghamri
# Date            : 16-11-2018
# Version         : 1.0
# Usage           : Compile using Cmake.
# Notes           : Main takes two inputs: input file path and output file path.
# C++_version     : C++14
# //TODO          : ...
# ==============================================================================
*/
#include "CsvInOut.hpp"

// main variables
// Data container object
CsvClass Data;

int main(int args, char** argv) {
    // Main takes two inputs: input file path and output file path.
    // Argument number verification
    if (args == 3) {
        printf("'OK' Arguments provided.\n");
    } else {
        printf("Missing main arguments.\n");
        return EXIT_FAILURE;
    }
    //Assigne the input file path.
    Data.ReadData(argv[1]);
    //use GetData method to retrieve data
    //Write to a file the filtered data
    Data.WriteData(Data.FilterData(), argv[2]);
    return EXIT_SUCCESS;
}
